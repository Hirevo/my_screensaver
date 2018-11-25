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

static void animate(Window *window, Context *ctx, vec_t *entities, bool filled)
{
    size_t size = lvec_size(entities);

    for (size_t i = 0; i < size; i++) {
        Entity *elem = entities->arr[i];
        elem->pos.x += elem->dir.x;
        elem->pos.y += elem->dir.y;
        if ((elem->pos.x + FADE_HALF_SIZE) >= ctx->width)
            elem->dir.x = -fabsf(elem->dir.x);
        else if ((elem->pos.x - FADE_HALF_SIZE) < 0)
            elem->dir.x = fabsf(elem->dir.x);
        if ((elem->pos.y + FADE_HALF_SIZE) >= ctx->height)
            elem->dir.y = -fabsf(elem->dir.y);
        else if ((elem->pos.y - FADE_HALF_SIZE) < 0)
            elem->dir.y = fabsf(elem->dir.y);
        if (filled)
            draw_filled_circle(ctx, (Vec){elem->pos.x, elem->pos.y},
                FADE_HALF_SIZE - 1, elem->color);
        else
            draw_circle(ctx, (Vec){elem->pos.x, elem->pos.y},
                FADE_HALF_SIZE - 1, elem->color);
    }
}

static void main_routine(Window *window, Context *ctx, vec_t *entities)
{
    sfEvent ev;
    bool filled = true;

    while (sfRenderWindow_isOpen(window->win)) {
        if (sfRenderWindow_pollEvent(window->win, &ev)) {
            if (ev.type == sfEvtClosed ||
                (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape))
                sfRenderWindow_close(window->win);
            else if (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyT)
                filled = !filled;
        }
        sfRenderWindow_clear(window->win, col(0, 0, 0, 255));
        fade_context(ctx, FADE_TRAIL);
        animate(window, ctx, entities, filled);
        sfTexture_updateFromPixels(
            ctx->texture, ctx->pixels, ctx->width, ctx->height, 0, 0);
        sfRenderWindow_drawSprite(window->win, ctx->sprite, 0);
        sfRenderWindow_display(window->win);
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

int main_fade(void)
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

    srand(time(0) * getpid());
    if (window.win == 0 || ctx.pixels == 0 || ctx.sprite == 0 ||
        ctx.texture == 0)
        return 84;
    vec_t *entities = lvec_with_capacity(FADE_DENSITY);
    for (int i = 0; i < FADE_DENSITY; i++) {
        Entity entity = create_fade_entity(&ctx);
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
