#ifndef _BIND_H_
#define _BIND_H_
#include <util/m_string.h>
#include <util/m_list.h>

typedef struct {
    str *bind_name;
    str *bind_value;
} C_BIND;

typedef struct {
    str *bind_name;
    str *bind_value;
    List *bind_children;
    unsigned int children_count;
} BIND;

typedef struct {
    BIND **binds;
    unsigned int bind_count;
} BIND_APP;

typedef enum {
    BIND_OK,
    BIND_CANT_PARS_USER_NAME,
}BIND_STATUS;

BIND *Bind_create_bind(str *name, str *value);
BIND_STATUS Bind_add_parent(BIND *b, BIND_APP *app);
BIND_STATUS Bind_add_child(BIND *b, str *name, str *value);
BIND_STATUS Bind_remove_child(BIND *b, str *name);
BIND_STATUS Bind_delete_bind(BIND *b, BIND_APP *app);
void Bind_show_help();
void Bind_show_binds(BIND_APP *app);

BIND_APP *Bind_load_binds_from_disk();
BIND_STATUS Bind_save_binds_on_disk(BIND_APP *app);

char *Bind_get_error();


#endif //_BIND_H_
