#include "../headers/bind.h"
#include <unistd.h>
#include <stdio.h>

#define null NULL

static const char *PATH_TO_CONF = "/home/%s/.local/share/bindhelper/resources/bindconf.cfg";
BIND_STATUS bind_status;
BIND_APP *app;

static str *get_pathtoconf() {

    char *name;
    name = getlogin();
    if (name == NULL) {
        bind_status = BIND_CANT_PARS_USER_NAME;
    }
    char buf[200];
    snprintf(buf, 200, PATH_TO_CONF, name);
    str *path = STR(buf, path);

    return path;
}

BIND_APP *Bind_load_binds_from_disk() {
    app = malloc(sizeof(BIND_APP));
    str *path = get_pathtoconf();

    if (path == null) {
        return null;
    }

    FILE *f = fopen(path->str, "r");
    str_free(path);
    if (f == NULL) {
        f = fopen(path->str, "w");
        fclose(f);
        return app;
    }
    char buf[200];
    List *config = list_create(0, M_STRING);

    while(!feof(f)) {
        char buf[200];
        fgets(buf, 200, f);
        list_add(config, buf);
    }

    fclose(f);

    BIND *b;

    str *bufs;
    str *bufs_val;

    for(int i = 0; i < config->len; i++) {
        bufs = STR(list_get(config, i++), bufs);

        if (str_starts_with(bufs, "-p")) {
            bufs = str_new_val(list_get(config, i++), buf);
            bufs = str_remove_all(bufs, '\n');
            bufs_val = STR(list_get(config, i++), bufs_val);
            bufs_val = str_remove_all(bufs_val, '\n');
            b = Bind_create_bind(bufs, bufs_val);
            bufs = str_new_val(bufs, list_get(config, i++));

            while (str_starts_with(bufs, "-c")) {
                bufs = str_new_val(bufs, list_get(config, i++));
                bufs = str_remove_all(bufs, '\n');
                bufs_val = str_new_val(bufs, list_get(config, i++));
                bufs_val = str_remove_all(bufs_val, '\n');
                Bind_add_child(b, bufs, bufs_val);
                bufs = str_new_val(bufs, list_get(config, i++));
            }
            Bind_add_parent(b, app);
        }
    }

    str_free(bufs);
    str_free(bufs_val);
    list_free_all(config);

    return app;
}

BIND_STATUS Bind_save_binds_on_disk(BIND_APP *app) {

    



    return BIND_OK;
}

char *Bind_get_error() {
    switch (bind_status) {
        case BIND_CANT_PARS_USER_NAME: {
            return "Can't find user login (function getlogin() return null)";
        }
        case BIND_OK: {
            return "Status OK";
        }
    }
    bind_status = BIND_OK;
}
