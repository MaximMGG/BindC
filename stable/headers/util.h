#ifndef _UTIL_H_
#define _UTIL_H_


char **string_split(char *str, char pattern, int *list_c);
int string_starts_with(char *str, char *pattern);
char *string_remove_all(char *str, char symbol);

#endif //_UTIL_H_
