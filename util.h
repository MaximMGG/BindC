#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PATH_TO_CONIFG "resources/config.xml"
#define PATH_TO_TRANLATOR "https://translate.google.com/?hl=ru&sl=en&tl=ru&text=%s%%0A&op=translate"

typedef struct {
    char *name;
    char *value;
} ChildBind;

typedef struct Bind {
    char *name;
    char *value;
    ChildBind **child;
    int children_count;
    int children_length;
} Bind, *pBind;


typedef struct {
    char *dir;
    Bind **binds;
    int binds_count;
    int binds_length;
} User;


// Binds func
Bind** create_user_bind(User *u);
Bind * add_bind(User *u, char *name, char *value);
ChildBind * add_child_bind(User *u, char *parent_name, char *name, char *value);
int delete_bind(User *u, char *name);
int delete_child_bind(User *u, char *parent_name, char *name);
//End binds bunc
