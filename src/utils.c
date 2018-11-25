/*
** EPITECH PROJECT, 2018
** my_screensaver
** File description:
** utils
*/

#include "screensaver.h"
#include <SFML/Graphics.h>
#include <math.h>

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