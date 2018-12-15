/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** utils
*/

#include "screensaver.h"
#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>

sfVector2u v2u(unsigned int x, unsigned int y)
{
    return (sfVector2u){
        .x = x,
        .y = y,
    };
}

sfVector2f v2f(float x, float y)
{
    return (sfVector2f){
        .x = x,
        .y = y,
    };
}

sfColor col(u8 r, u8 g, u8 b, u8 a)
{
    return (sfColor){
        .r = r,
        .g = g,
        .b = b,
        .a = a,
    };
}

double dist(double x1, double y1, double x2, double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int irand(void)
{
    return arc4random() % RAND_MAX;
}

float frand(void)
{
    return arc4random() % RAND_MAX;
}

int imax(int a, int b)
{
    return (a > b) ? a : b;
}

int imin(int a, int b)
{
    return (a < b) ? a : b;
}

int iclamp(int val, int min, int max)
{
    return imax(imin(val, max), min);
}
