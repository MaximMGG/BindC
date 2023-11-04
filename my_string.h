#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


typedef struct m_string{
    char *str;
    unsigned int length;
    void * (*fm_strcpy) (struct m_string *str, char *s);
} str;

str *cr_str(char *s);
char * str_set(char *s);
void * str_cpy(str *to, str *from);
char * str_format(char *s, ...);
void str_distroy(str *s);

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


