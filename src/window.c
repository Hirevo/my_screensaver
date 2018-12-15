/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** window
*/

#include "screensaver.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Window create_window(sfUint32 style, size_t width, size_t height)
{
    Window window = {
        .mode =
            (sfVideoMode){
                .width = width,
                .height = height,
                .bitsPerPixel = 32,
            },
        .win = 0,
    };

    window.win =
        sfRenderWindow_create(window.mode, "my_screensaver", style, 0);
    if (window.win == 0)
        return window;
    sfRenderWindow_setFramerateLimit(window.win, 60);
    sfRenderWindow_setVerticalSyncEnabled(window.win, sfTrue);
    sfRenderWindow_setMouseCursorVisible(window.win, sfFalse);
    return window;
}

Context create_context(size_t width, size_t height, Color fill_color)
{
    Context ctx = {
        .width = width,
        .height = height,
        .pixels = malloc(sizeof(u8) * (width * height * 4)),
        .texture = sfTexture_create(width, height),
        .sprite = sfSprite_create(),
    };
    if (!ctx.pixels || !ctx.texture || !ctx.sprite)
        return ctx;
    for (size_t y = 0; y < height; y++)
        for (size_t x = 0; x < width; x++)
            put_pixel(&ctx, (Vec){x, y}, fill_color);
    sfSprite_setTexture(ctx.sprite, ctx.texture, sfTrue);
    return ctx;
}

int disp_routines()
{
    for (size_t i = 0; i < 5; i++)
        printf("%zu: %s\n", i, descriptions[i]);
    return 0;
}

int main(int ac, char **av)
{
    if (ac != 2)
        return 84;
    if (strcmp(av[1], "-d") == 0)
        return disp_routines();
    int id = atoi(av[1]);
    if (id > 4)
        return 84;
    return routines[id]();
}
