/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** fade
*/

#include "screensaver.h"
#include "vec.h"
#include <SFML/Window.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static Entity create_trailing_entity(Context *ctx)
{
    return (Entity){
        .pos = (Vec){irand() % ctx->width, irand() % ctx->height},
        .dir =
            (Vec){
                ceilf(frand() * TRAILING_DOUBLE_SPEED / RANDF_MAX -
                    TRAILING_SPEED),
                ceilf(frand() * TRAILING_DOUBLE_SPEED / RANDF_MAX -
                    TRAILING_SPEED),
            },
        .size = (Vec){TRAILING_SIZE, TRAILING_SIZE},
        .color = (Color){irand() % 256, irand() % 256, irand() % 256, 255},
    };
}

static void animate(Context *ctx, vec_t *entities, bool filled)
{
    size_t size = lvec_size(entities);

    for (size_t i = 0; i < size; i++) {
        Entity *elem = entities->arr[i];
        elem->pos.x += elem->dir.x;
        elem->pos.y += elem->dir.y;
        if ((elem->pos.x + BLURRED_TRAILING_HALF_SIZE) >= ctx->width)
            elem->dir.x = -fabsf(elem->dir.x);
        else if ((elem->pos.x - BLURRED_TRAILING_HALF_SIZE) < 0)
            elem->dir.x = fabsf(elem->dir.x);
        if ((elem->pos.y + BLURRED_TRAILING_HALF_SIZE) >= ctx->height)
            elem->dir.y = -fabsf(elem->dir.y);
        else if ((elem->pos.y - BLURRED_TRAILING_HALF_SIZE) < 0)
            elem->dir.y = fabsf(elem->dir.y);
        if (filled)
            draw_filled_circle(ctx, (Vec){elem->pos.x, elem->pos.y},
                BLURRED_TRAILING_HALF_SIZE - 1, elem->color);
        else
            draw_circle(ctx, (Vec){elem->pos.x, elem->pos.y},
                BLURRED_TRAILING_HALF_SIZE - 1, elem->color);
    }
}

static void blur_context(Context *ctx, Kernel *kernel)
{
    const size_t half_size = kernel->size / 2;
    Context ret = (Context){
        .texture = ctx->texture,
        .sprite = ctx->sprite,
        .width = ctx->width,
        .height = ctx->height,
        .pixels = malloc(sizeof(u8) * (ctx->width * ctx->height * 4)),
    };

    for (size_t y = 0; y < ctx->height; y++)
        for (size_t x = 0; x < ctx->width; x++) {
            float final[4] = {0};
            for (size_t i = 0; i < kernel->size; i++) {
                Color pixel =
                    get_pixel(ctx, (Vec){x, imax(y + (i - half_size), 0)});
                final[0] += ((float)(pixel.r)) * kernel->kernel[i];
                final[1] += ((float)(pixel.g)) * kernel->kernel[i];
                final[2] += ((float)(pixel.b)) * kernel->kernel[i];
                final[3] += ((float)(pixel.a)) * kernel->kernel[i];
            }
            final[0] = fminf(roundf(final[0]), 255.f);
            final[1] = fminf(roundf(final[1]), 255.f);
            final[2] = fminf(roundf(final[2]), 255.f);
            final[3] = fminf(roundf(final[3]), 255.f);
            put_pixel(&ret, (Vec){x, y},
                (Color){final[0], final[1], final[2], final[3]});
        }

    for (size_t y = 0; y < ctx->height; y++)
        for (size_t x = 0; x < ctx->width; x++) {
            float final[4] = {0};
            for (size_t i = 0; i < kernel->size; i++) {
                Color pixel =
                    get_pixel(&ret, (Vec){imax(x + (i - half_size), 0), y});
                final[0] += ((float)(pixel.r)) * kernel->kernel[i];
                final[1] += ((float)(pixel.g)) * kernel->kernel[i];
                final[2] += ((float)(pixel.b)) * kernel->kernel[i];
                final[3] += ((float)(pixel.a)) * kernel->kernel[i];
            }
            final[0] = fminf(roundf(final[0]), 255.f);
            final[1] = fminf(roundf(final[1]), 255.f);
            final[2] = fminf(roundf(final[2]), 255.f);
            final[3] = fminf(roundf(final[3]), 255.f);
            put_pixel(ctx, (Vec){x, y},
                (Color){final[0], final[1], final[2], final[3]});
        }

    free(ret.pixels);
}

static const char *fs =
    "uniform sampler2D texture;\n"
    "uniform float kernel[59];\n"
    "uniform vec2 sizes;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 final = vec4(0);\n"
    "    vec2 res = 1.0 / sizes;\n"
    "    for (int i = -29; i <= 29; i++) {\n"
    "        vec4 pixel = texture2D(\n"
    "            texture,\n"
    "            gl_TexCoord[0].xy + vec2(float(i) * res.x, 0)\n"
    "        );\n"
    "        final += pixel * kernel[i + 29];\n"
    "    }\n"
    "    gl_FragColor = final;\n"
    "}\n";

static const char *fs2 =
    "uniform sampler2D texture;\n"
    "uniform float kernel[59];\n"
    "uniform vec2 sizes;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 final = vec4(0);\n"
    "    vec2 res = 1.0 / sizes;\n"
    "    for (int i = -29; i <= 29; i++) {\n"
    "        vec4 pixel = texture2D(\n"
    "            texture,\n"
    "            gl_TexCoord[0].xy + vec2(0, float(i) * res.y)\n"
    "        );\n"
    "        final += pixel * kernel[i + 29];\n"
    "    }\n"
    "    gl_FragColor = final;\n"
    "}\n";

static void main_routine(
    Window *window, Context *ctx, vec_t *entities, Kernel *kernel)
{
    sfEvent ev;
    bool filled = true;

    sfShader *shader = sfShader_createFromMemory(0, 0, fs);
    sfShader *shader2 = sfShader_createFromMemory(0, 0, fs2);
    sfRenderStates states = {
        .shader = shader,
        .blendMode = sfBlendAlpha,
        .transform = sfTransform_Identity,
        .texture = 0,
    };
    sfRenderStates states2 = {
        .shader = shader2,
        .blendMode = sfBlendAlpha,
        .transform = sfTransform_Identity,
        .texture = 0,
    };
    sfRenderTexture *target =
        sfRenderTexture_create(ctx->width, ctx->height, false);
    sfSprite *spr = sfSprite_create();
    sfSprite_setTexture(spr, sfRenderTexture_getTexture(target), sfTrue);
    sfShader_setCurrentTextureParameter(shader, "texture");
    sfShader_setFloatUniformArray(
        shader, "kernel", kernel->kernel, kernel->size);
    sfShader_setVec2Uniform(
        shader, "sizes", (sfGlslVec2){ctx->width, ctx->height});
    sfShader_setCurrentTextureParameter(shader2, "texture");
    sfShader_setFloatUniformArray(
        shader2, "kernel", kernel->kernel, kernel->size);
    sfShader_setVec2Uniform(
        shader2, "sizes", (sfGlslVec2){ctx->width, ctx->height});
    while (sfRenderWindow_isOpen(window->win)) {
        if (sfRenderWindow_pollEvent(window->win, &ev)) {
            if (ev.type == sfEvtClosed ||
                (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape))
                sfRenderWindow_close(window->win);
            else if (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyT)
                filled = !filled;
        }
        sfRenderWindow_clear(window->win, col(0, 0, 0, 255));
        fade_context(ctx, BLURRED_TRAILING_TRAIL);
        animate(ctx, entities, filled);
        sfTexture_updateFromPixels(
            ctx->texture, ctx->pixels, ctx->width, ctx->height, 0, 0);
        sfRenderTexture_drawSprite(target, ctx->sprite, &states);
        sfRenderTexture_display(target);
        sfRenderWindow_drawSprite(window->win, spr, &states2);
        sfRenderWindow_display(window->win);
    }
}

static void destroy_routine(
    Window *window, Context *ctx, vec_t *entities, Kernel *kernel)
{
    size_t size = lvec_size(entities);

    for (size_t i = 0; i < size; i++)
        free(entities->arr[i]);
    lvec_drop(entities);
    free(ctx->pixels);
    sfTexture_destroy(ctx->texture);
    sfSprite_destroy(ctx->sprite);
    sfRenderWindow_destroy(window->win);
    free(kernel->kernel);
}

static Kernel create_kernel(void)
{
    Kernel kernel = (Kernel){
        .size = ceilf(6.f * BLURRED_TRAILING_SIGMA - 1.f),
        .sigma = BLURRED_TRAILING_SIGMA,
        .kernel = 0,
    };
    int half_size = kernel.size / 2;
    kernel.kernel = calloc(kernel.size, sizeof(float));
    const float precomputed = 2 * kernel.sigma * kernel.sigma;
    const float expr = sqrtf(1 / (M_PI * precomputed));

    for (int x = -half_size; x <= half_size; x++) {
        const float fx = x;
        const float coef = expr * powf(M_E, -((fx * fx) / precomputed));
        kernel.kernel[x + half_size] = coef;
    }

    float sum = 0;
    for (size_t x = 0; x < kernel.size; x++)
        sum += kernel.kernel[x];
    for (size_t x = 0; x < kernel.size; x++)
        kernel.kernel[x] /= sum;
    return kernel;
}

int main_blurred_trailing(void)
{
    size_t count;
    const sfVideoMode *modes = sfVideoMode_getFullscreenModes(&count);
    printf("Available sizes:\n");
    for (size_t i = 0; i < count; i++)
        printf("%s (%u, %u)\n", i ? "   " : "=> ", modes[i].width,
            modes[i].height);
    Window window =
        create_window(sfFullscreen, modes[0].width, modes[0].height);
    // create_window(sfClose, 1920, 1080);
    Context ctx = create_context(
        window.mode.width, window.mode.height, (Color){0, 0, 0, 255});
    Kernel kernel = create_kernel();

    vec_t *entities = lvec_with_capacity(BLURRED_TRAILING_DENSITY);
    if (window.win == 0 || ctx.pixels == 0 || ctx.sprite == 0 ||
        ctx.texture == 0) {
        destroy_routine(&window, &ctx, entities, &kernel);
        return 84;
    }
    for (int i = 0; i < BLURRED_TRAILING_DENSITY; i++) {
        Entity entity = create_trailing_entity(&ctx);
        Entity *allocated = malloc(sizeof(Entity));
        if (allocated == 0) {
            destroy_routine(&window, &ctx, entities, &kernel);
            return 84;
        }
        memcpy(allocated, &entity, sizeof(Entity));
        lvec_push_back(entities, 1, allocated);
    }
    main_routine(&window, &ctx, entities, &kernel);
    destroy_routine(&window, &ctx, entities, &kernel);
    return (0);
}
