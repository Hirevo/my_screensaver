/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** alloc
*/

#include "screensaver.h"
#include <stdlib.h>
#include <string.h>

Entity create_fade_entity(Context *ctx)
{
    return (Entity){
        .pos = (Vec){rand() % ctx->width, rand() % ctx->height},
        .dir =
            (Vec){
                ceilf(((float)(rand())) * FADE_DOUBLE_SPEED / ((float)(RAND_MAX)) -
                    FADE_SPEED),
                ceilf(((float)(rand())) * FADE_DOUBLE_SPEED / ((float)(RAND_MAX)) -
                    FADE_SPEED),
            },
        .size = (Vec){FADE_SIZE, FADE_SIZE},
        .color = (Color){rand() % 256, rand() % 256, rand() % 256, 255},
    };
}