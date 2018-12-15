/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** morph
*/

#include "screensaver.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static Entity create_meta(Context *ctx)
{
    return (Entity){
        .pos = (Vec){irand() % ctx->width, irand() % ctx->height},
        .dir =
            (Vec){
                frand() * META_GPU_SPEED / RANDF_MAX - 1.5,
                frand() * META_GPU_SPEED / RANDF_MAX - 1.5,
            },
        .color = (Color){irand() % 128, irand() % 128, irand() % 128, 255},
        .size = (Vec){10, 10},
    };
}

static void morph_move(Context *ctx, vec_t *entities)
{
    size_t size = lvec_size(entities);

    for (size_t i = 0; i < size; i++) {
        Entity *elem = entities->arr[i];
        elem->pos.x += elem->dir.x;
        elem->pos.y += elem->dir.y;
        if (elem->pos.x >= ctx->width || elem->pos.x < 0)
            elem->dir.x = -elem->dir.x;
        if (elem->pos.y >= ctx->height || elem->pos.y < 0)
            elem->dir.y = -elem->dir.y;
    }
}

static void morph_tick(
    Window *window, Context *ctx, vec_t *entities, sfRenderStates *states)
{
    char *str = 0;

    morph_move(ctx, entities);
    sfRenderWindow_clear(window->win, sfBlack);
    sfShader_setVec2Uniform((sfShader *)(states->shader), "sizes",
        (sfGlslVec2){ctx->width, ctx->height});
    for (size_t i = 0; i < META_GPU_DENSITY; i++) {
        Entity *elem = entities->arr[i];
        asprintf(&str, "lights[%zu].pos", i);
        sfShader_setVec2Uniform((sfShader *)(states->shader), str,
            (sfGlslVec2){elem->pos.x, elem->pos.y});
        free(str);
        asprintf(&str, "lights[%zu].color", i);
        sfShader_setVec4Uniform((sfShader *)(states->shader), str,
            (sfGlslVec4){
                elem->color.r, elem->color.g, elem->color.b, elem->color.a});
        free(str);
    }
    sfRenderWindow_drawSprite(window->win, ctx->sprite, states);
    sfRenderWindow_display(window->win);
}

static const char *fs =
    "struct Light {\n"
    "    vec2 pos;\n"
    "    vec4 color;\n"
    "};\n"
    "\n"
    "uniform Light lights[40];\n"
    "uniform vec2 sizes;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 final = vec4(0);\n"
    "    vec2 res = 1.0 / sizes;\n"
    "    for (int i = 0; i < 40; i++) {\n"
    "        float d = distance(\n"
    "            gl_TexCoord[0].xy * sizes.xy,\n"
    "            lights[i].pos.xy\n"
    "         );\n"
    "        final += 100.0 * (lights[i].color / 255.0) / (d * 2.0);\n"
    "    }\n"
    "    gl_FragColor = final;\n"
    "}\n";

static void main_routine(Window *window, Context *ctx, vec_t *entities)
{
    sfEvent ev;
    sfShader *shader = sfShader_createFromMemory(0, 0, fs);
        sfRenderStates states = {
        .shader = shader,
        .blendMode = sfBlendAlpha,
        .transform = sfTransform_Identity,
        .texture = 0,
    };
    while (sfRenderWindow_isOpen(window->win)) {
        if (sfRenderWindow_pollEvent(window->win, &ev))
            if (ev.type == sfEvtClosed ||
                (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape))
                sfRenderWindow_close(window->win);
        morph_tick(window, ctx, entities, &states);
    }
}

static void destroy_routine(Window *window, Context *ctx, vec_t *entities)
{
    size_t size = lvec_size(entities);

    for (size_t i = 0; i < size; i++)
        free(entities->arr[i]);
    lvec_drop(entities);
    free(ctx->pixels);
    sfTexture_destroy(ctx->texture);
    sfSprite_destroy(ctx->sprite);
    sfRenderWindow_destroy(window->win);
}

int main_meta_gpu(void)
{
    size_t count;
    const sfVideoMode *modes = sfVideoMode_getFullscreenModes(&count);
    printf("Available sizes:\n");
    for (size_t i = 0; i < count; i++)
        printf("%s (%u, %u)\n", i ? "   " : "=> ", modes[i].width,
            modes[i].height);
    Window window =
        create_window(sfFullscreen, modes[0].width, modes[0].height);
    Context ctx = create_context(
        window.mode.width, window.mode.height, (Color){0, 0, 0, 255});

    if (window.win == 0 || ctx.pixels == 0 || ctx.sprite == 0 ||
        ctx.texture == 0)
        return 84;
    vec_t *entities = lvec_with_capacity(META_GPU_DENSITY);
    for (int i = 0; i < META_GPU_DENSITY; i++) {
        Entity entity = create_meta(&ctx);
        Entity *allocated = malloc(sizeof(Entity));
        if (allocated == 0)
            return 84;
        memcpy(allocated, &entity, sizeof(Entity));
        lvec_push_back(entities, 1, allocated);
    }
    main_routine(&window, &ctx, entities);
    destroy_routine(&window, &ctx, entities);
    return (0);
}
