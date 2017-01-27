//
// Created by Timothy Barnard on 27/01/2017.
//
#include <stdio.h>
#include "area_of_circle.h"

float area_of_circle(int num1) {

    float area;
    int radius = num1;

    printf("Finding area of Circle");

    printf("Radius: %d\n",radius);

    area = (float) (PI * radius * radius);

    printf("Area: %f",area);

    return area;
}