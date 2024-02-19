#include "../headers/bind.h"
#include <unistd.h>
#include <stdio.h>
#include "../headers/util.h"

#define null NULL

static const char *PATH_TO_CONF = "/home/%s/.local/share/bindhelper/resources/bindconf.cfg";
//                                  /home/maxim/.local/share/bindhelper/resources
static char *full_path;
BIND_STATUS bind_status;
BIND_APP *app;

static void Bind_free_child(C_BIND *b) {
    free(b->bind_name);
    free(b->bind_value);
    free(b);
    b = null;
}

static void Bind_free_bind(BIND *b) {
    if (b->children_count > 0) {
        for (int i = 0; i < b->children_count; i++) {
            Bind_free_child(b->bind_children[i]);
        }
        free(b->bind_children);
        b->children_count = 0;
    }
    free(b->bind_name);
    free(b->bind_value);
    free(b);
    b = null;
}


BIND *Bind_create_bind(char *name, char *value) {
    BIND *new = (BIND *)malloc(sizeof(*new));
    new->bind_name = (char *) malloc(sizeof(char) * strlen(name) + 1);
    new->bind_value = (char *) malloc(sizeof(char) * strlen(value) + 1);
    strcpy(new->bind_name, name);
    strcpy(new->bind_value, name);
    new->bind_children = NULL;
    new->children_count = 0;

    return new;
}

BIND_STATUS Bind_add_parent(BIND *b, BIND_APP *app) {
    if (app->bind_count == 0) {
        app->binds = (BIND **) malloc(sizeof(BIND *));
        app->binds[0] = b;
    } else {
        app->binds = (BIND **) realloc(app->binds, sizeof(BIND *) * app->bind_count + 1);
        app->binds[app->bind_count] = b;
    }
    app->bind_count++;
    return BIND_OK;
}


BIND_STATUS Bind_add_child(BIND *b, char *name, char *value) {
    if (b->children_count == 0) {
        b->bind_children = (C_BIND **) malloc(sizeof(C_BIND *));
    } else {
        b->bind_children = (C_BIND **) realloc(b->bind_children, sizeof(C_BIND *) * b->children_count + 1);
    }
    C_BIND *new_child = (C_BIND *) malloc(sizeof(C_BIND));
    new_child->bind_name = (char *) malloc(sizeof(char) * strlen(name) + 1);
    new_child->bind_value = (char *) malloc(sizeof(char) * strlen(value) + 1);
    strcpy(new_child->bind_name, name);
    strcpy(new_child->bind_value, name);
    b->bind_children[b->children_count] = new_child;
    b->children_count++;

    return BIND_OK;
}

BIND_STATUS Bind_remove_child(BIND *b, char *name) {
    for(int i = 0; i < b->children_count; i++) {
        if (strcmp(b->bind_children[i]->bind_name, name) == 0) {
            Bind_free_child(b->bind_children[i]);
            if (i != b->children_count) {
                for(int j = i; j < b->children_count - 1; j++) {
                    b->bind_children[j] = b->bind_children[j + 1];
                }
                b->children_count--;
                break;
            }
        } 
    }
    return BIND_OK;
}

BIND_STATUS Bind_delete_bind(char *b, BIND_APP *app) {
    for(int i = 0; i < app->bind_count; i++) {
        if (strcmp(b, app->binds[i]->bind_name) == 0) {
            Bind_free_bind(app->binds[i]);
            if (i != app->bind_count - 1) {
                for(int j = i; j < app->bind_count - 1; j++) {
                    app->binds[j] = app->binds[j + 1];
                }
            }
        }
    }
    app->bind_count--;
    return BIND_OK;
}

static char *get_pathtoconf() {
    char *name;
    name = getlogin();
    if (name == NULL) {
        bind_status = BIND_CANT_PARS_USER_NAME;
    }
    char buf[200];
    snprintf(buf, 200, PATH_TO_CONF, name);
    char *path = (char *) malloc(sizeof(char) * strlen(buf) + 1);
    strcpy(path, buf);
    full_path = malloc(strlen(path));
    strcpy(full_path, path);

    return path;
}


BIND_APP *Bind_load_binds_from_disk() {
    app = malloc(sizeof(BIND_APP));
    char *path = get_pathtoconf();

    if (path == null) {
        return null;
    }

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        f = fopen(path, "w");
        fclose(f);
        free(path);
        return app;
    }
    free(path);
    char buf[200];
    List *config = list_create(0, M_STRING);

    while(!feof(f)) {
        char buf[200];
        fgets(buf, 200, f);
        list_add(config, buf);
    }

    fclose(f);

    BIND *b = null;

    char *bufs;
    char *bufs_val;
    char *lb;
    bufs = (char *) list_get(config, 0);
    for(int i = 1; i < config->len; ) {
        if (config->len < 2) {
            break;
        }

        if (string_starts_with(bufs, "-p")) {
            bufs = list_get(config, i++);
            bufs = string_remove_all(bufs, '\n');
            bufs_val = list_get(config, i);
            i++;
            bufs_val = string_remove_all(bufs_val, '\n');
            b = Bind_create_bind(bufs, bufs_val);
            bufs = list_get(config, i++);

            while (string_starts_with(bufs, "-c")) {
                bufs = list_get(config, i++);
                bufs = string_remove_all(bufs, '\n');
                bufs_val = list_get(config, i++);
                bufs_val = string_remove_all(bufs_val, '\n');
                Bind_add_child(b, bufs, bufs_val);
                bufs = list_get(config, i++);
            }
            Bind_add_parent(b, app);
        }
    }

    list_free_all(config);

    return app;
}

BIND_STATUS Bind_save_binds_on_disk(BIND_APP *app) {
    FILE *f = fopen(full_path, "w");
    if (f == null) {
        bind_status = BIND_CANT_OPEN_CONFIG_FILE;
        return BIND_CANT_OPEN_CONFIG_FILE;
    }
    
    for(int i = 0; i < app->bind_count; i++) {
        fputs("-p\n", f);
        fputs(app->binds[i]->bind_name, f);
        fputs("\n", f);
        fputs(app->binds[i]->bind_value, f);
        fputs("\n", f);
        for(int j = 0; j < app->binds[i]->children_count; j++) {
            fputs("-c\n", f); 
            fputs(app->binds[i]->bind_children[j]->bind_name, f);
            fputs("\n", f);
            fputs(app->binds[i]->bind_children[j]->bind_value, f);
            fputs("\n", f);
        }
    }
    fclose(f);
    return BIND_OK;
}

char *Bind_get_bindval(char *name, BIND_APP *app) {
    for(int i = 0; i < app->bind_count; i++) {
        if (strcmp(name, app->binds[i]->bind_name) == 0) {
            return app->binds[i]->bind_value;
        }
    }
    return null;
}


char *Bind_get_bindchildval(char *pname, char *cname, BIND_APP *app) {
    for(int i = 0; i < app->bind_count; i++) {
        if (strcmp(pname, app->binds[i]->bind_name) == 0) {
            BIND *b = app->binds[i];
            for (int j = 0; j < b->children_count; j++) {
                if (strcmp(cname, b->bind_children[j]->bind_name) == 0) {
                    return b->bind_children[j]->bind_value;
                } 
            }
        }
    }
    return null;
}

char *Bind_get_error() {
    switch (bind_status) {
        case BIND_CANT_PARS_USER_NAME: {
            return "Can't find user login (function getlogin() return null)";
        }
        case BIND_OK: {
            return "Status OK";
        }
        case BIND_CANT_OPEN_CONFIG_FILE: {
            return "Can't open config file, maybe path incorrent";
        }
        case BIND_CANT_FIND_BIND_WITH_NAME: {
            return "Can't find bind with than name";
        }
    }
    bind_status = BIND_OK;
}

void Bind_destroy_app(BIND_APP *app) {
    for(int i = 0; i < app->bind_count; i++) {
        Bind_free_bind(app->binds[i]);
    }
    free(app->binds);
    free(app);
    free(full_path);
}

#define SHOW_HELP   "flag 'b' -----------------------------\n"                                    \
                    "b 'bindname' -> open site\n"                                                 \
                    "b 'bindname 'childbindname' -> open child site'\n"                           \
                    "b cp 'bindname' 'bindvalue' -> create new parent bind\n"                     \
                    "b cc 'bindname' 'childbindname' 'childvalue' -> create new child bind\n"     \
                    "b dp 'bindname' -> delete bind and all chilern binds\n"                      \
                    "b dc 'bindname' 'childbindname' -> delete child bind\n"                      


void Bind_show_help() {
    printf("%s\n", SHOW_HELP);
}

void Bind_show_binds(BIND_APP *app) {
    puts("------------------------------------------------------------------------------------");
    for(int i = 0; i < app->bind_count; i++) {
        printf("Parent bind name - %s, value - %s\n",   app->binds[i]->bind_name, 
                                                        app->binds[i]->bind_value);
        for(int j = 0; j < app->binds[i]->children_count; j++) {
            printf("    Child bind name - %s, value - %s\n", 
                                app->binds[i]->bind_children[j]->bind_name,
                                app->binds[i]->bind_children[j]->bind_value);

        }
        puts("=====>");
    }
    puts("------------------------------------------------------------------------------------");
}

BIND *Bind_get_bind_byname(BIND_APP *app, char *name) {
    for(int i = 0; i < app->bind_count; i++) {
        if (strcmp(app->binds[i]->bind_name, name) == 0) {
            return app->binds[i];
        }
    }
    return null;
}
