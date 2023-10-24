#include "util.h"


Bind** create_user_bind(User *u) {
    Bind **head_bind;
    head_bind = malloc(sizeof(Bind) * u->lenths_of_binds);

    for(int i = 0; i < u->lenths_of_binds; i++) {
        head_bind[i] = malloc(sizeof(Bind));
    }

    return head_bind;
}

Bind * add_bind(User *u, char *name, char *value, char *parent_name, enum F f) {
    if (f == PARENT) {
        Bind b = {name, value, NULL};
        u->binds[u->binds_count] = &b;
        u->binds_count++;
    } else {
        for(int i = 0; i < u->binds_count; i++){
            if (u->binds[i]->name == parent_name) {
                Bind b = {name, value, NULL};
                Bind *parent_bind = u->binds[i];
                if (parent_bind->child == NULL) {
                    // parent_bind->child = ;
                }
            }
        }
    }
}


Bind **create_children(User *u) {
    Bind **children;
    children = malloc(sizeof(Bind) * u->lenths_of_binds);
    for(int i = 0; i < u->lenths_of_binds; i++){
        children[i] = malloc(sizeof(Bind));
    }
    return children;
}
