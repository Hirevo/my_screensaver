/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** screensaver
*/

#pragma once

#include <SFML/Graphics.h>
#include <math.h>

#define TAU (2 * M_PI)

#define BLOB_SIZE (128)
#define BLOB_HALF_SIZE (BLOB_SIZE / 2)
#define BLOB_SPEED (10.0)
#define BLOB_DENSITY (500)

#define FADE_SIZE 32
#define FADE_HALF_SIZE (FADE_SIZE / 2)
#define FADE_SPEED (10.0)
#define FADE_DOUBLE_SPEED (FADE_SPEED * 2)
#define FADE_DENSITY (500)
#define FADE_TRAIL (10)

#define MORPH_SPEED (10.0)
#define MORPH_DENSITY (10)

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

typedef struct Blob {
    Vec pos;
    Vec dir;
    Vec size;
    Color color;
    Context ctx;
} Blob;

typedef struct Window {
    sfVideoMode mode;
    sfRenderWindow *win;
} Window;

/*
** utils.c
*/
sfVector2u v2u(unsigned int, unsigned int);
sfVector2f v2f(float, float);
sfColor col(unsigned int, unsigned int, unsigned int, unsigned int);
double dist(double, double, double, double);

/*
** window.c
*/
Window create_window(sfUint32 style, size_t width, size_t height);
Context create_context(size_t width, size_t height, Color fill_color);

/*
** blobs/alloc.c
*/
Blob create_blob(Context *ctx);

/*
** blobs/blobs.c
*/
int main_blobs(void);

/*
** fade/fade.c
*/
int main_fade(void);

/*
** fade/alloc.c
*/
Entity create_fade_entity(Context *ctx);

/*
** draw.c
*/
void put_pixel(Context *ctx, Vec pos, Color c);
void fade_context(Context *ctx, int decr);
void draw_circle(Context *ctx, Vec pos, int radius, Color color);
void draw_filled_circle(Context *ctx, Vec pos, int radius, Color color);

/*
** morph/morph.c
*/
int main_morph(void);

static int (*routines[])(void) = {
    &main_blobs,
    &main_morph,
    &main_fade,
};

static const char *descriptions[] = {
    "Translucent lights flying across the screen, bouncing off the edges.",
    "Colored luminescent blobs merging upon clustering, bouncing off the "
    "edges.",
    "particles flying across the screen, bouncing off the edges and leaving "
    "trails.",
};
