#ifndef _MY_STR_H_

#define _MY_STR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


typedef struct m_string{
    char *str;
    unsigned int length;
    void * (*fm_strcpy) (struct m_string *str, char *s);
} str;

typedef struct {
    char * (*pset_string) (const char *s);
} m_setS;

//create new str * with allocation memory for char *
str *cr_str(char *s);
/*
 * formattind string like in printf function, but return formatted str *
*/
str * str_format(str *main, ...);
//free all memory from str *
void str_distroy(str *s);
//comparing to strings, return 0 if not same return 1 if the same
int str_cmp(str *one, str *two);
//return length of string 
unsigned int str_length(char *);
/*
 * return pointer to memory copyes from "from" to "to" mamory allocaion
*/

void * str_mem_cpy(str *to, str *from, char *old_string, unsigned int size);
void * _str_mem_cpy(char *to, char *from, unsigned int size);
/*
 * return string from target surraunded to symbols: from_symbol and to_symbol
 * f.e. <path>/usr/home/code/test.c</path> -> /usr/home/code/test.c
*/
char * get_str_between(char *target, char from_symbol, char to_symbol);
/*
 *Concatinate two string with symbol between them
 *return new char *
*/
str * str_concat(str *first, str *second, char symbol);


int ** getPacOfDicimal();
char * mapIntToString(int buf);

enum T{
    BIL = 1000000000,
    MIL_100 = 100000000,
    MIL_10 = 10000000,
    MIL = 1000000,
    SOU_100 = 100000,
    SOU_10 = 10000,
    SOU = 1000,
    HAN_100 = 100,
    HAN_10 = 10
};

#endif // _MY_STR_H_
