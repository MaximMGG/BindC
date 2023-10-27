#include "util.h"


Bind** create_user_bind(User *u) {
    if (u->binds == NULL) {
        u->binds_length = 10;
        u->binds = malloc(sizeof(Bind) * u->binds_length);
    } else {
        return u->binds;
    }
    return u->binds;
}


Bind * add_bind(User *u, char *name, char *value) {
    u->binds[u->binds_count] = malloc(sizeof(Bind));
    u->binds[u->binds_count]->name = name;
    u->binds[u->binds_count]->value = value;
    u->binds[u->binds_count]->child = NULL;
    u->binds_count++;

    if (u->binds_count == 10) {
        u->binds_length *= 2;
        u->binds = realloc(u->binds, sizeof(Bind) * u->binds_length);
    }
}


ChildBind * add_child_bind(User *u, char *parent_name, char *name, char *value) {
    for(int i = 0; i < u->binds_count; i++){
        if (strcmp(u->binds[i]->name, parent_name)) {
            Bind *tmp = u->binds[i];

            if (tmp->child == NULL) {
                tmp->children_length = 10;
                tmp->children_count = 0;
                tmp->child = malloc(sizeof(ChildBind) * tmp->children_length);
                tmp->child[0] = malloc(sizeof(ChildBind));
                tmp->child[0]->name = name;
                tmp->child[0]->value = value;
            } else {
                tmp->child[tmp->children_count] = malloc(sizeof(ChildBind));
                tmp->child[tmp->children_count]->name = name;
                tmp->child[tmp->children_count]->value = value;
            }
        }
    }
    return 0;
}


int delete_bind(User *u, char *name) {
    int res = 1;
    for(int i = 0; i < u->binds_count; i++){

        if (strcmp(u->binds[i]->name, name)) {
            free(u->binds[i]);
            res = 0;
            if (i == u->binds_count - 1) {
                u->binds_count--;
                break;
            } else {
                for(int j = i; j < u->binds_count; j++){
                    u->binds[j] = u->binds[j + 1];
                }
                u->binds_count--;
            }
        }
    }
    return res;
}



int delete_child_bind(User *u, char *parent_name, char *name) {

    int res = 1;

    for(int i = 0; i < u->binds_count; i++){
        if (strcmp(u->binds[i]->name, parent_name)) {
            Bind *tmp = u->binds[i];
            for(int j = 0; j < tmp->children_count; j++){
                if (strcmp(tmp->child[j]->name, name)) {
                    free(tmp->child[j]);
                    if (j == tmp->children_count - 1) {
                        tmp->children_count--;
                        break;
                    } else {
                        for(int k = j; k < tmp->children_count; k++){
                            tmp->child[k] = tmp->child[k + 1];
                        }
                        tmp->children_count--;
                        break;
                    }
                } 
            }
        }
    }

    return res;
}
    
