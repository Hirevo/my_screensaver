/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** screensaver
*/

#pragma once

#include <SFML/Graphics.h>
#include <math.h>
#include <limits.h>

#define TAU (2 * M_PI)
#define RANDF_MAX ((float)(INT_MAX))

#define BLOB_SIZE (128)
#define BLOB_HALF_SIZE (BLOB_SIZE / 2)
#define BLOB_SPEED (10.0)
#define BLOB_DENSITY (500)

#define TRAILING_SIZE (32)
#define TRAILING_HALF_SIZE (TRAILING_SIZE / 2)
#define TRAILING_SPEED (10.0)
#define TRAILING_DOUBLE_SPEED (TRAILING_SPEED * 2)
#define TRAILING_DENSITY (1000)
#define TRAILING_TRAIL (10)

#define META_SPEED (10.0)
#define META_DENSITY (10)

#define BLURRED_TRAILING_SIZE (32)
#define BLURRED_TRAILING_HALF_SIZE (BLURRED_TRAILING_SIZE / 2)
#define BLURRED_TRAILING_SPEED (10.0)
#define BLURRED_TRAILING_DOUBLE_SPEED (BLURRED_TRAILING_SPEED * 2)
#define BLURRED_TRAILING_DENSITY (1000)
#define BLURRED_TRAILING_TRAIL (10)
#define BLURRED_TRAILING_SIGMA (10)

#define META_GPU_SPEED (10.0)
#define META_GPU_DENSITY (40)

typedef unsigned char u8;

typedef struct Context {
    sfTexture *texture;
    sfSprite *sprite;
    size_t width;
    size_t height;
    u8 *pixels;
} Context;

typedef struct Color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Color;

typedef struct Vec {
    float x;
    float y;
} Vec;

typedef struct Entity {
    Vec pos;
    Vec dir;
    Vec size;
    Color color;
} Entity;

typedef struct Light {
    Vec pos;
    Vec dir;
    Vec size;
    Color color;
    Context ctx;
} Light;

typedef struct Kernel {
    size_t size;
    float sigma;
    float *kernel;
} Kernel;

typedef struct Window {
    sfVideoMode mode;
    sfRenderWindow *win;
} Window;

/*
** utils.c
*/
sfVector2u v2u(unsigned int, unsigned int);
sfVector2f v2f(float, float);
sfColor col(u8, u8, u8, u8);
double dist(double, double, double, double);
int irand(void);
float frand(void);
int imax(int, int);
int imin(int, int);
int iclamp(int, int, int);

/*
** window.c
*/
Window create_window(sfUint32 style, size_t width, size_t height);
Context create_context(size_t width, size_t height, Color fill_color);

/*
** lights/mod.c
*/
int main_lights(void);

/*
** trailing/mod.c
*/
int main_trailing(void);

/*
** blurred_trailing/mod.c
*/
int main_blurred_trailing(void);

/*
** meta_gpu/mod.c
*/
int main_meta_gpu(void);

/*
** draw.c
*/
void put_pixel(Context *ctx, Vec pos, Color c);
Color get_pixel(Context *ctx, Vec pos);
void fade_context(Context *ctx, int decr);
void draw_circle(Context *ctx, Vec pos, int radius, Color color);
void draw_filled_circle(Context *ctx, Vec pos, int radius, Color color);

/*
** meta/mod.c
*/
int main_meta(void);

static int (*routines[])(void) = {
    &main_lights,
    &main_meta,
    &main_trailing,
    &main_blurred_trailing,
    &main_meta_gpu,
};

static const char *descriptions[] = {
    "Translucent lights flying across the screen, bouncing off the edges.",
    "Colored luminescent meta-lights merging upon clustering, bouncing off "
    "the edges.",
    "Particles flying across the screen, bouncing off the edges and leaving "
    "trails.",
    "Blurred particles flying across the screen, bouncing off the edges and "
    "leaving trails. (GPU Accelerated)",
    "Colored luminescent blobs merging upon clustering, bouncing off the "
    "edges. (GPU Accelerated)",
};
