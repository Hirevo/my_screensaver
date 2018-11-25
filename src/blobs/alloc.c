/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** alloc
*/

#include "screensaver.h"
#include <stdlib.h>
#include <string.h>

Blob create_blob(Context *ctx)
{
    const Vec size = (Vec){BLOB_SIZE, BLOB_SIZE};
    const Color color = {rand() % 256, rand() % 256, rand() % 256, 100};
    Blob blob = {
        .pos = (Vec){rand() % ctx->width, rand() % ctx->height},
        .dir =
            (Vec){
                ((float)(rand())) * BLOB_SPEED / ((float)(RAND_MAX)) - 1.5,
                ((float)(rand())) * BLOB_SPEED / ((float)(RAND_MAX)) - 1.5,
            },
        .size = size,
        .color = color,
        .ctx = create_context(size.x, size.y, color),
    };

    sfSprite_setPosition(
        blob.ctx.sprite, v2f(rand() % ctx->width, rand() % ctx->height));
    return blob;
}
