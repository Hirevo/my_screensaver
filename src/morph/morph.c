/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** morph
*/

#include "screensaver.h"
#include "vec.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static Entity create_morph(Context *ctx)
{
    return (Entity){
        .pos = (Vec){rand() % ctx->width, rand() % ctx->height},
        .dir =
            (Vec){
                ((float)(rand())) * MORPH_SPEED / ((float)(RAND_MAX)) - 1.5,
                ((float)(rand())) * MORPH_SPEED / ((float)(RAND_MAX)) - 1.5,
            },
        .color = (Color){rand() % 128, rand() % 128, rand() % 128, 255},
        .size = 10,
    };
}

static Color animate(vec_t *entities, Vec pos)
{
    size_t size = lvec_size(entities);
    sfVector3f color = {0, 0, 0};

    for (size_t i = 0; i < size; i++) {
        Entity *elem = entities->arr[i];
        float d = dist(pos.x, pos.y, elem->pos.x, elem->pos.y);
        color.x += 50 * elem->color.r / d;
        color.y += 50 * elem->color.g / d;
        color.z += 50 * elem->color.b / d;
    }
    return (Color){
        (color.x > 255) ? 255 : color.x,
        (color.y > 255) ? 255 : color.y,
        (color.z > 255) ? 255 : color.z,
        255,
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

static void morph_tick(Window *window, Context *ctx, vec_t *entities)
{
    morph_move(ctx, entities);
    sfRenderWindow_clear(window->win, sfBlack);
    for (int y = 0; y < ctx->height; y++)
        for (int x = 0; x < ctx->width; x++) {
            int idx = (x + y * ctx->width) * 4;
            Color color = animate(entities, (Vec){x, y});
            put_pixel(ctx, (Vec){x, y}, color);
        }
    sfTexture_updateFromPixels(
        ctx->texture, ctx->pixels, ctx->width, ctx->height, 0, 0);
    sfRenderWindow_drawSprite(window->win, ctx->sprite, 0);
    sfRenderWindow_display(window->win);
}

static void main_routine(Window *window, Context *ctx, vec_t *entities)
{
    sfEvent ev;

    while (sfRenderWindow_isOpen(window->win)) {
        if (sfRenderWindow_pollEvent(window->win, &ev))
            if (ev.type == sfEvtClosed ||
                (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape))
                sfRenderWindow_close(window->win);
        morph_tick(window, ctx, entities);
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

int main_morph(void)
{
    Window window = create_window(sfClose, 600, 600);
    Context ctx = create_context(
        window.mode.width, window.mode.height, (Color){0, 0, 0, 255});

    srand(time(0) * getpid());
    if (window.win == 0 || ctx.pixels == 0 || ctx.sprite == 0 ||
        ctx.texture == 0)
        return 84;
    vec_t *entities = lvec_with_capacity(MORPH_DENSITY);
    for (int i = 0; i < MORPH_DENSITY; i++) {
        Entity entity = create_morph(&ctx);
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
