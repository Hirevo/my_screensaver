/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** draw
*/

#include "screensaver.h"
#include <math.h>

size_t smax(size_t x, size_t y)
{
    return (x > y) ? x : y;
}

size_t smin(size_t x, size_t y)
{
    return (x < y) ? x : y;
}

size_t sclamp(size_t a, size_t x, size_t y)
{
    return smax(smin(a, y), x);
}

void put_pixel(Context *ctx, Vec pos, Color c)
{
    const long index = (pos.y * ctx->width + pos.x) * 4;

    if (pos.x >= 0 && pos.y >= 0 && pos.x < ctx->width &&
        pos.y < ctx->height) {
        ctx->pixels[index + 0] = c.r;
        ctx->pixels[index + 1] = c.g;
        ctx->pixels[index + 2] = c.b;
        ctx->pixels[index + 3] = c.a;
    }
}

Color get_pixel(Context *ctx, Vec pos)
{
    const long safe_x = sclamp(pos.x, 0, ctx->width - 1);
    const long safe_y = sclamp(pos.y, 0, ctx->height - 1);
    const long index = (safe_y * ctx->width + safe_x) * 4;

    return (Color){
        .r = ctx->pixels[index + 0],
        .g = ctx->pixels[index + 1],
        .b = ctx->pixels[index + 2],
        .a = ctx->pixels[index + 3],
    };
}

void fade_context(Context *ctx, int decr)
{
    for (size_t y = 0; y < ctx->height; y++)
        for (size_t x = 0; x < ctx->width; x++) {
            int tmp = (x + y * ctx->width) * 4;
            ctx->pixels[tmp + 0] = (ctx->pixels[tmp + 0] > decr) ?
                ctx->pixels[tmp + 0] - decr :
                0;
            ctx->pixels[tmp + 1] = (ctx->pixels[tmp + 1] > decr) ?
                ctx->pixels[tmp + 1] - decr :
                0;
            ctx->pixels[tmp + 2] = (ctx->pixels[tmp + 2] > decr) ?
                ctx->pixels[tmp + 2] - decr :
                0;
            ctx->pixels[tmp + 3] = 255;
        }
}

void draw_circle(Context *ctx, Vec pos, int radius, Color color)
{
    for (float angle = 0.0; angle <= (TAU + 0.05); angle += 0.05)
        put_pixel(ctx,
            (Vec){
                pos.x + round(cosf(angle) * radius),
                pos.y + round(sinf(angle) * radius),
            },
            color);
}

void draw_filled_circle(Context *ctx, Vec pos, int radius, Color color)
{
    for (int y = -radius; y <= radius; y++)
        for (int x = -radius; x <= radius; x++)
            if ((x * x) + (y * y) <= (radius * radius))
                put_pixel(ctx, (Vec){pos.x + x, pos.y + y}, color);
}
