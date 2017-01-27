//
// Created by Timothy Barnard on 27/01/2017.
//

#include <string.h>
#include <stdio.h>
#include "roman_num.h"

char* get_numeral(int val) {

    int a = 13;
    char* return_val = "";
    static int decimals[] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
    static char* roman[] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};

    for (int i = a; --i >= 0;){

        if ( val > decimals[a] ) {
            return_val = roman[i];
            break;
        }
    }

    return  return_val;
}

char* get_roman_num( int number) {

    int x = number;
    //char roman_num[] = "";
    //char* return_val  = "";
    char myString[255] = "";

    //return_val = get_numeral(number);
    do {
        strcat(myString, get_numeral(x) );
        printf("%d\n",x);
        printf("%s\n",myString);

        //strcat(return_val, roman_num);

        x = number - x;

    } while ( x > 0 );

    return  myString;

}