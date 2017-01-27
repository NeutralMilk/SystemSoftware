#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "area_of_rec.h"
#include "area_of_circle.h"
#include "area_of_triangle.h"
#include "roman_num.h"


int main(int argc, char *argv[]) {


    if( strcmp(argv[1], "Circle")  == 0 ) {
        area_of_circle(atoi(argv[2]));

    } else if (strcmp(argv[1], "Rectangle") == 0) {
        area_of_rectangle(atoi(argv[2]), atoi(argv[3]));

    } else if (strcmp( argv[1], "Triangle" ) == 0) {
        area_of_triangle(atoi(argv[2]),atoi(argv[3]));
    } else if (strcmp(argv[1], "Roman") ==0) {
        char* roman = get_roman_num(atoi(argv[2]));
        printf("Roman numeral from: %d = %s",atoi(argv[2]),roman);
    }
}
