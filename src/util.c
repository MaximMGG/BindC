#include "../headers/util.h"
#include "../headers/my_string.h"



int compare_string(char *one, char *two)  {
    unsigned int one_length = str_length(one);
    unsigned int two_length = str_length(two);

    if (one_length != two_length) return 0;
    else 
        for(int i = 0; i < one_length; i++) 
            if (one[i] != two[i]) return 0;
    return 1;
}


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
    return u->binds[u->binds_count - 1];
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


//list func

List *CreateUserConfig() {
    FILE *f = fopen(PATH_TO_CONFIG, "w");
    
    List *list = malloc(sizeof(*list));
    
    list->line = malloc(sizeof(char *) * 10);

    for(int i = 0; i < 10; i++){
        list->line[i] = malloc(sizeof(char) * 64);
    }

    strcpy(list->line[0], "<PathToDir>\n");
    strcpy(list->line[1], "\t<path>p</path>\n");
    strcpy(list->line[2], "</PathToDir>\n");
    strcpy(list->line[3], "<Bind>\n");
    strcpy(list->line[4], "\t<ParentBind>\n");
    strcpy(list->line[5], "\t\t<ChildBinds>\n");
    strcpy(list->line[6], "\t\t</ChildBinds>\n");
    strcpy(list->line[7], "\t</ParentBind>\n");
    strcpy(list->line[8], "</Bind>\n");

    for (int i = 0; i < 9; i++){
        fputs(list->line[i], f);
    }

    list->length = 9;

    fclose(f);
    return list;
}


List * ReadAllLineFromConfig() {
    FILE *f = fopen(PATH_TO_CONFIG, "r");

    List *list;

    if (f == NULL) {
        list = CreateUserConfig(); 
        return list;
    }

    int line_count = 100;
        
    list = malloc(sizeof(*list));
    list->line = malloc(sizeof(char*) * line_count);
    list->length = 0;

    for(int i = 0; !feof(f); i++){ 
        list->line[i] = malloc(sizeof(char) * 100);

        fgets(list->line[i], 100, f);
        list->length++;

        if (list->length >= line_count) {
            line_count *= 2;
            list->line = realloc(list->line, sizeof(char*) * line_count);
        }
    }

    fclose(f);
    return list;
} 
    
//list func end


char ** parsUserEnter(char *user_enter, int length) {

    char **parsed_enter = malloc(sizeof(char *) * 10);
    int command = 0;

    char *buffer = malloc(sizeof(char) * 100);

    for(int i = 0, j = 0; i < length; i++, j++){
        if (user_enter[i] == ' ' || user_enter[i] == '\0' || user_enter[i] == '\n') {
            buffer[j] = '\0';
            parsed_enter[command] = malloc(sizeof(*buffer));
            strcpy(parsed_enter[command], buffer);
            command++;
            j = -1;
        } else {
            buffer[j] = user_enter[i];
        }
    }

    free(buffer);

    return parsed_enter;
}



void showTranslation(const char *variable) {
    str *path = cr_str(PATH_TO_TRANSLATOR);
    path = str_format(path, variable);
    path = str_format(cr_str("google-chrome %s"), path->str); 
    //char *tr_link = str_format(PATH_TO_TRANSLATOR, variable);
    //char *chrome = str_format("google-chrome %s", tr_link);
    system(path->str);
}
