/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** blobs
*/

#include "screensaver.h"
#include "vec.h"
#include <SFML/Window.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static Light create_light(Context *ctx)
{
    const Vec size = (Vec){BLOB_SIZE, BLOB_SIZE};
    const Color color = {irand() % 256, irand() % 256, irand() % 256, 100};
    Light light = {
        .pos = (Vec){irand() % ctx->width, irand() % ctx->height},
        .dir =
            (Vec){
                frand() * BLOB_SPEED / RANDF_MAX - 1.5,
                frand() * BLOB_SPEED / RANDF_MAX - 1.5,
            },
        .size = size,
        .color = color,
        .ctx = create_context(size.x, size.y, color),
    };

    sfSprite_setPosition(
        light.ctx.sprite, v2f(irand() % ctx->width, irand() % ctx->height));
    return light;
}

static void animate(Window *window, vec_t *lights)
{
    size_t size = lvec_size(lights);

    for (size_t i = 0; i < size; i++) {
        Light *light = lights->arr[i];
        light->pos.x += light->dir.x;
        light->pos.y += light->dir.y;
        if ((light->pos.x + BLOB_HALF_SIZE) > window->mode.width)
            light->dir.x = -fabsf(light->dir.x);
        else if ((light->pos.x + BLOB_HALF_SIZE) < 0)
            light->dir.x = fabsf(light->dir.x);
        else if ((light->pos.y + BLOB_HALF_SIZE) > window->mode.height)
            light->dir.y = -fabsf(light->dir.y);
        else if ((light->pos.y + BLOB_HALF_SIZE) < 0)
            light->dir.y = fabsf(light->dir.y);
        sfSprite_setPosition(
            light->ctx.sprite, (sfVector2f){light->pos.x, light->pos.y});
        sfRenderWindow_drawSprite(window->win, light->ctx.sprite, 0);
    }
}

static void draw_light(Light *light, bool filled)
{
    memset(light->ctx.pixels, 0, sizeof(u8) * BLOB_SIZE * BLOB_SIZE * 4);
    if (filled)
        draw_filled_circle(&light->ctx, (Vec){BLOB_HALF_SIZE, BLOB_HALF_SIZE},
            BLOB_HALF_SIZE - 1, light->color);
    else
        draw_circle(&light->ctx, (Vec){BLOB_HALF_SIZE, BLOB_HALF_SIZE},
            BLOB_HALF_SIZE - 1, light->color);
    sfTexture_updateFromPixels(light->ctx.texture, light->ctx.pixels,
        light->ctx.width, light->ctx.height, 0, 0);
}

static void switch_mode(bool *filled, vec_t *lights)
{
    size_t size = lvec_size(lights);

    *filled = !*filled;
    for (size_t i = 0; i < size; i++) {
        Light *light = lights->arr[i];
        draw_light(light, *filled);
    }
}

static void main_routine(Window *window, Context *ctx, vec_t *lights)
{
    sfEvent ev;
    bool filled = true;

    (void)(ctx);
    switch_mode(&filled, lights);
    while (sfRenderWindow_isOpen(window->win)) {
        if (sfRenderWindow_pollEvent(window->win, &ev)) {
            if (ev.type == sfEvtClosed ||
                (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape))
                sfRenderWindow_close(window->win);
            else if (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyT)
                switch_mode(&filled, lights);
        }
        sfRenderWindow_clear(window->win, col(0, 0, 0, 1));
        animate(window, lights);
        sfRenderWindow_display(window->win);
    }
}

static void destroy_routine(Window *window, Context *ctx, vec_t *lights)
{
    size_t size = lvec_size(lights);

    for (size_t i = 0; i < size; i++) {
        Light *light = lights->arr[i];
        free(light->ctx.pixels);
        sfTexture_destroy(light->ctx.texture);
        sfSprite_destroy(light->ctx.sprite);
        free(light);
    }
    lvec_drop(lights);
    free(ctx->pixels);
    sfTexture_destroy(ctx->texture);
    sfSprite_destroy(ctx->sprite);
    sfRenderWindow_destroy(window->win);
}

int main_lights(void)
{
    size_t count;
    sfVideoMode mode = sfVideoMode_getFullscreenModes(&count)[0];
    Window window = create_window(sfFullscreen, mode.width, mode.height);
    Context ctx = create_context(
        window.mode.width, window.mode.height, (Color){0, 0, 0, 255});

    if (window.win == 0 || ctx.pixels == 0 || ctx.sprite == 0 ||
        ctx.texture == 0)
        return (84);
    vec_t *lights = lvec_with_capacity(BLOB_DENSITY);
    for (int i = 0; i < BLOB_DENSITY; i++) {
        Light light = create_light(&ctx);
        Light *allocated = malloc(sizeof(Light));
        if (allocated == 0)
            return 84;
        memcpy(allocated, &light, sizeof(Light));
        lvec_push_back(lights, 1, allocated);
    }
    main_routine(&window, &ctx, lights);
    destroy_routine(&window, &ctx, lights);
    return (0);
}
