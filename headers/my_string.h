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

str *cr_str(char *s);
char * str_set(char *s);
void * str_cpy(str *to, str *from);
char * str_format(char *s, ...);
void str_distroy(str *s);
int str_cmp(str *one, str *two);

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
