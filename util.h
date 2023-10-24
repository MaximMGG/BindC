#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PATH_TO_CONIFG "resources/config.xml"
#define PATH_TO_TRANLATOR "https://translate.google.com/?hl=ru&sl=en&tl=ru&text=%s%%0A&op=translate"


typedef struct Bind {
    char *name;
    char *value;
    Bind **child;
} Bind, *pBind;


typedef struct {
    char *dir;
    Bind **binds;
    int lenths_of_binds;
    int binds_count;
} User;

enum F{
    PARENT,
    CHILD
};

// Binds func
Bind** create_user_bind(User *u);
Bind* add_bind(User *u, char *name, char *value, char *parent_name, enum F f);
Bind* delete_bind(User *u, char *name, Bind *b);
//End binds bunc
