#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "my_string.h"
#include <dirent.h>
#include <unistd.h>

#define PATH_TO_CONFIG "/home/%s/.local/share/bindhelper/resources/config.xml"
#define PATH_TO_TRANSLATOR "\"https://translate.google.com/?hl=ru&sl=en&tl=ru&text=%s%%0A&op=translate\" &"


typedef struct {
    char **line;
    int length;
    unsigned int count;
} List;

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
    char *cur_file;
    Bind **binds;
    int binds_count;
    int binds_length;
} User;


// Binds func
Bind** create_user_bind(User *u);
Bind** set_binds_from_config(List *config, User *u);
Bind * add_bind(User *u, char *name, char *value);
ChildBind * add_child_bind(User *u, char *parent_name, char *name, char *value);
int delete_bind(User *u, char *name);
int delete_child_bind(User *u, char *parent_name, char *name);
//prepare List for writting info in user config.xml
List * prepare_user_config(User *u);
//write user config in config.xml
void write_user_config(User *u);
//show all binds
void show_all_bind(User *u);

void callParentBind(User *u, char *bind);
void callChildBind(User *u, char *p_bind, char *c_bind);


//End binds func

// List func

List *ReadAllLineFromConfig();
List *CreateUserConfig();
List *add(char *val, List *cur);

//End List func

//Flag realisation funcs

void showTranslation(const char *);
// for flag "f s"
int setCurrentFile(char *file_name, User *u);
//for flag f show
void showAllFilesInDir(User *u);
//crete the new file in user dir
void createNewFile(User *u, char *name);

//set in User *u path to dir with files
void setUserDir(User *u, char *full_path);
//appent word with translation in current file
void appentWord(User *u, char *word);

//End of flag realisation funcs


List * parsUserEnter(char *user_enter, int length);
List * list_create();


//utility func

//compare two strings return 1 if the same and 0 if not
int compare_string(char *, char *);


// end utility func

#endif
