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

static void animate(Window *window, vec_t *blobs)
{
    size_t size = lvec_size(blobs);

    for (size_t i = 0; i < size; i++) {
        Blob *blob = blobs->arr[i];
        blob->pos.x += blob->dir.x;
        blob->pos.y += blob->dir.y;
        if ((blob->pos.x + BLOB_HALF_SIZE) > window->mode.width)
            blob->dir.x = -fabsf(blob->dir.x);
        else if ((blob->pos.x + BLOB_HALF_SIZE) < 0)
            blob->dir.x = fabsf(blob->dir.x);
        else if ((blob->pos.y + BLOB_HALF_SIZE) > window->mode.height)
            blob->dir.y = -fabsf(blob->dir.y);
        else if ((blob->pos.y + BLOB_HALF_SIZE) < 0)
            blob->dir.y = fabsf(blob->dir.y);
        sfSprite_setPosition(
            blob->ctx.sprite, (sfVector2f){blob->pos.x, blob->pos.y});
        sfRenderWindow_drawSprite(window->win, blob->ctx.sprite, 0);
    }
}

static void draw_blob(Blob *blob, bool filled)
{
    memset(blob->ctx.pixels, 0, sizeof(u8) * BLOB_SIZE * BLOB_SIZE * 4);
    if (filled)
        draw_filled_circle(&blob->ctx, (Vec){BLOB_HALF_SIZE, BLOB_HALF_SIZE},
            BLOB_HALF_SIZE - 1, blob->color);
    else
        draw_circle(&blob->ctx, (Vec){BLOB_HALF_SIZE, BLOB_HALF_SIZE},
            BLOB_HALF_SIZE - 1, blob->color);
    sfTexture_updateFromPixels(blob->ctx.texture, blob->ctx.pixels,
        blob->ctx.width, blob->ctx.height, 0, 0);
}

static void switch_mode(bool *filled, vec_t *blobs)
{
    size_t size = lvec_size(blobs);

    *filled = !*filled;
    for (size_t i = 0; i < size; i++) {
        Blob *blob = blobs->arr[i];
        draw_blob(blob, *filled);
    }
}

static void main_routine(Window *window, Context *ctx, vec_t *blobs)
{
    sfEvent ev;
    bool filled = true;

    switch_mode(&filled, blobs);
    while (sfRenderWindow_isOpen(window->win)) {
        if (sfRenderWindow_pollEvent(window->win, &ev)) {
            if (ev.type == sfEvtClosed ||
                (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape))
                sfRenderWindow_close(window->win);
            else if (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyT)
                switch_mode(&filled, blobs);
        }
        sfRenderWindow_clear(window->win, col(0, 0, 0, 1));
        animate(window, blobs);
        sfRenderWindow_display(window->win);
    }
}

static void destroy_routine(Window *window, Context *ctx, vec_t *blobs)
{
    size_t size = lvec_size(blobs);

    for (size_t i = 0; i < size; i++) {
        Blob *blob = blobs->arr[i];
        free(blob->ctx.pixels);
        sfTexture_destroy(blob->ctx.texture);
        sfSprite_destroy(blob->ctx.sprite);
        free(blob);
    }
    lvec_drop(blobs);
    free(ctx->pixels);
    sfTexture_destroy(ctx->texture);
    sfSprite_destroy(ctx->sprite);
    sfRenderWindow_destroy(window->win);
}

int main_blobs(void)
{
    size_t count;
    sfVideoMode mode = sfVideoMode_getFullscreenModes(&count)[0];
    Window window = create_window(sfFullscreen, mode.width, mode.height);
    Context ctx = create_context(
        window.mode.width, window.mode.height, (Color){0, 0, 0, 255});

    srand(time(0) * getpid());
    if (window.win == 0 || ctx.pixels == 0 || ctx.sprite == 0 ||
        ctx.texture == 0)
        return (84);
    vec_t *blobs = lvec_with_capacity(BLOB_DENSITY);
    for (int i = 0; i < BLOB_DENSITY; i++) {
        Blob blob = create_blob(&ctx);
        Blob *allocated = malloc(sizeof(Blob));
        if (allocated == 0)
            return 84;
        memcpy(allocated, &blob, sizeof(Blob));
        lvec_push_back(blobs, 1, allocated);
    }
    main_routine(&window, &ctx, blobs);
    destroy_routine(&window, &ctx, blobs);
    return (0);
}
