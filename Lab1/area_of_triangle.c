//
// Created by Timothy Barnard on 27/01/2017.
//
#include <stdio.h>
#include "area_of_triangle.h"

int area_of_triangle(int num1, int num2) {

    int area;
    int base = num1;
    int height = num2;

    printf("Finding area of Triangle");


    printf("Base: %d\n",base);
    printf("Height: %d\n",height);

    area = base * height/2;

    printf("Area: %d",area);

    return area;

}