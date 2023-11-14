#include "../headers/util.h"
#include "../headers/my_string.h"



int compare_string(char *one, char *two)  {
    unsigned int one_length = str_length(one);
    unsigned int two_length = str_length(two);

    if (one_length != two_length) return 0;
    else 
        for(int i = 0; i < one_length - 1; i++) 
            if (one[i] != two[i]) return 0;
    return 1;
}


Bind** create_user_bind(User *u) {
    if (u->binds == NULL) {
        u->binds_length = 10;
        u->binds = malloc(sizeof(Bind **));
    } else {
        return u->binds;
    }
    return u->binds;
}


Bind * add_bind(User *u, char *name, char *value) {
    u->binds[u->binds_count] = malloc(sizeof(Bind *));
    u->binds[u->binds_count]->name = name;
    u->binds[u->binds_count]->value = value;
    u->binds[u->binds_count]->child = NULL;
    u->binds_count++;

    if (u->binds_count == 10) {
        u->binds_length *= 2;
        u->binds = realloc(u->binds, sizeof(Bind *) * u->binds_length);
    }
    return u->binds[u->binds_count - 1];
}


ChildBind * add_child_bind(User *u, char *parent_name, char *name, char *value) {
    for(int i = 0; i < u->binds_count; i++){
        if (!strcmp(u->binds[i]->name, parent_name)) {
            Bind *tmp = u->binds[i];

            if (tmp->child == NULL) {
                tmp->children_length = 10;
                tmp->children_count = 0;
                tmp->child = malloc(sizeof(ChildBind *) * tmp->children_length);
                tmp->child[0] = malloc(sizeof(ChildBind *));
                tmp->child[0]->name = name;
                tmp->child[0]->value = value;
                tmp->children_count++;
            } else {
                tmp->child[tmp->children_count] = malloc(sizeof(ChildBind *));
                tmp->child[tmp->children_count]->name = name;
                tmp->child[tmp->children_count]->value = value;
                tmp->children_count++;
            }

            if (tmp->children_count >= 10) {
                tmp->children_length <<= 1;
                tmp->child = realloc(tmp->child, sizeof(Bind *) * tmp->children_length);
            }
        }
    }
    return NULL;
}


int delete_bind(User *u, char *name) {
    int res = 1;
    for(int i = 0; i < u->binds_count; i++){

        if (!strcmp(u->binds[i]->name, name)) {
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
        if (!strcmp(u->binds[i]->name, parent_name)) {
            Bind *tmp = u->binds[i];
            for(int j = 0; j < tmp->children_count; j++){
                if (!strcmp(tmp->child[j]->name, name)) {
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
    
    list->line = malloc(sizeof(char *) * 5);

    for(int i = 0; i < 5; i++){
        list->line[i] = malloc(sizeof(char) * 64);
    }

    strcpy(list->line[0], "<PathToDir>\n");
    strcpy(list->line[1], "\t<path>p</path>\n");
    strcpy(list->line[2], "</PathToDir>\n");
    strcpy(list->line[3], "<Bind>\n");
    strcpy(list->line[4], "</Bind>\n");

    for (int i = 0; i < 5; i++){
        fputs(list->line[i], f);
    }

    list->length = 5;

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


List * parsUserEnter(char *user_enter, int length) {

    List *list = list_create();

    char *buffer = malloc(sizeof(char) * 100);

    if (user_enter[0] == 'a') {
        list = add("a", list);
        for(int i = 2, j = 0; i < length; i++, j++) {
            buffer[j] = user_enter[i];
        }
        list = add(buffer, list);
        free(buffer);
        return list;
    }

    for(int i = 0, j = 0; i < length; i++, j++){
        if (user_enter[i] == ' ' || user_enter[i] == '\0' || user_enter[i] == '\n') {
            buffer[j] = '\0';
            list = add(buffer, list);
            j = -1;
        } else {
            buffer[j] = user_enter[i];
        }
    }

    free(buffer);

    return list;
}

List *list_create() {
    List *new = (List *) malloc(sizeof(*new));
    
    new->line = (char **) malloc(sizeof(char *) * 10);
    new->length = 10;
    new->count = 0;

    return new;
}

List *add(char *val, List *cur) {
    int len = str_length(val);
    cur->line[cur->count] = (char *) malloc(sizeof(char) * len);
    _str_mem_cpy(cur->line[cur->count], val, len);
    cur->count++;

    if (cur->count >= cur->length) {
        cur->length <<= 1;
        cur->line = realloc(cur->line, sizeof(char *) * cur->length);
    }
    return cur;
}

List *getAllFilesFromDir(User *u) {
    DIR *dir = opendir(u->dir);
    struct dirent *di;
    List *dirContent = list_create();

    if (dir) {
        while((di = readdir(dir)) != NULL) {
            dirContent = add(di->d_name, dirContent);
        }
    } else {
        fprintf(stderr, "ERROR: file dir name %s doesn't exitst", u->dir);
    }

    closedir(dir);
    return dirContent;
}

void showAllFilesInDir(User *u) {
    List *list = getAllFilesFromDir(u);
    for(int i = 0; i < list->count; i++) {
        printf("%s\n", list->line[i]);
    }
    free(list);
}

void showTranslation(const char *variable) {
    str *path = cr_str(PATH_TO_TRANSLATOR);
    path = str_format(path, variable);
    path = str_format(cr_str("google-chrome %s"), path->str); 
    //char *tr_link = str_format(PATH_TO_TRANSLATOR, variable);
    //char *chrome = str_format("google-chrome %s", tr_link);
    system(path->str);
}

int setCurrentFile(char *file_name, User *u) {
    List *file_in_dir = getAllFilesFromDir(u);
    for(int i = 0; i < file_in_dir->count; i++) {
        if (!strcmp(file_name, file_in_dir->line[i])) {
            u->cur_file = file_name;
            free(file_in_dir);
            return 1;
        }
    }
    free(file_in_dir);
    return 0;
}

void setUserDir(User *u, char *full_path) {
    DIR *dir = opendir(full_path);
    if (dir == NULL) {
       fprintf(stderr, "Incorrect path to dir -> %s, try to write agane full path to your dir\n", full_path);
    } else {
        u->dir = full_path;
    }
    closedir(dir);
}



void appentWord(User *u, char *word) {
    FILE *f = fopen(str_concat(cr_str(u->dir), cr_str(u->cur_file), 0)->str, "a");

    fputs(word, f);

    fclose(f);
}

void createNewFile(User *u, char *name) {
    str *file_name = cr_str(name);
    str *path_to_file = cr_str(u->dir);
    path_to_file = str_concat(path_to_file, file_name, 0);
    FILE *f = fopen(path_to_file->str, "w");

    str_distroy(file_name);
    str_distroy(path_to_file);

    fclose(f);
}


Bind** set_binds_from_config(List *config, User *u) {

    Bind **u_b = create_user_bind(u);
    u->binds = u_b;

    for(int i = 0; i < config->length; i++) {

        if (!strcmp(config->line[i], "\t<ParentBind>\n")) {
            i++;
            char *bname = get_str_between(config->line[i++], '>', '<');
            char *bvalue = get_str_between(config->line[i++], '>', '<');
            add_bind(u, bname, bvalue);

            for(int j = i; j < config->length; j++) {
                if (!strcmp(config->line[j], "\t\t\t<ChildBind>\n")) {
                    j++;
                    char *bcname = get_str_between(config->line[j++], '>', '<');
                    char *bcvalue = get_str_between(config->line[j++], '>', '<');
                    add_child_bind(u, bname, bcname, bcvalue);
                }
                if (!strcmp(config->line[j], "\t\t</ChildBinds>\n")) {
                    break;
                }
            }
        }

    }
    return u_b;
}

void write_user_config(User *u) {
    List *conf = prepare_user_config(u);
    FILE *f = fopen(PATH_TO_CONFIG, "w");

    for(int i = 0; i < conf->count; i++) {
        fputs(conf->line[i], f);
    }

    fclose(f);
    free(conf);
}

List * prepare_user_config(User *u) {
    List *conf = list_create();
    
    add(cr_str("<PathToDir>\n")->str, conf);
    add(str_concat(str_format(cr_str("\t<path>%s</path>"), u->dir), cr_str("\n"), 0)->str, conf);
    add(cr_str("</PathToDir>\n")->str, conf);
    
    add(cr_str("<Bind>\n")->str, conf);

    for(int i = 0; i < u->binds_count; i++) {
        add(cr_str("\t<ParentBind>\n")->str, conf);
        add(str_concat(str_format(cr_str("\t\t<name>%s</name>\n"), u->binds[i]->name), cr_str("\n"), 0)->str, conf);
        add(str_concat(str_format(cr_str("\t\t<value>%s</value>\n"), u->binds[i]->value), cr_str("\n"), 0)->str, conf);
        add(cr_str("\t\t<ChildBinds>\n")->str, conf);

        for(int j = 0; j < u->binds[i]->children_count; j++) {
            add(cr_str("\t\t\t<ChildBind>\n")->str, conf);
            add(str_concat(
                    str_format(
                        cr_str("\t\t\t\t<name>%s</name>\n"), u->binds[i]->child[j]->name), cr_str("\n"), 0)->str, conf);
            add(str_concat(
                    str_format(
                        cr_str("\t\t\t\t<value>%s</value>\n"), u->binds[i]->child[j]->value), cr_str("\n"), 0)->str, conf);
            add(cr_str("\t\t\t</ChildBind>\n")->str, conf);
        }

        add(cr_str("\t\t</ChildBinds>\n")->str, conf);
        add(cr_str("\t</ParentBind>\n")->str, conf);
    }

    add(cr_str("</Bind>\n")->str, conf);
    

    return conf;
}


void show_all_bind(User *u) {
    puts("-----------------------------------------------------");
    for(int i = 0; i < u->binds_count; i++) {
        printf("parent bind name -> %s, value -> %s\n", u->binds[i]->name, u->binds[i]->value);
        for(int j = 0; j < u->binds[i]->children_count; j++) {
            printf("\tchild bind name -> %s, value -> %s\n", u->binds[i]->child[j]->name, u->binds[i]->child[j]->value);
        }
    }
    puts("-----------------------------------------------------");
}


void callParentBind(User *u, char *bind) {
    for (int i = 0; i < u->binds_count; i++) {
        if (!strcmp(u->binds[i]->name, bind)) {
            str *call = str_concat(cr_str("google-chrome"), cr_str(u->binds[i]->value), ' ');
            call = str_concat(call, cr_str("&"), ' ');
            system(call->str);
            return;
        }
    }
    printf("You don't binding with name: %s\n", bind);
}

void callChildBind(User *u, char *p_bind, char *c_bind) {
    for (int i = 0; i < u->binds_count; i++) {
        if (!strcmp(u->binds[i]->name, p_bind)) {
            ChildBind *cb = u->binds[i]->child[0];
            for(int j = 0; j < u->binds[i]->children_count; j++) {
                if(!strcmp(cb->name, c_bind)) {
                    str *call = str_concat(cr_str("google-chrome"), cr_str(cb->value), ' ');
                    call = str_concat(call, cr_str("&"), ' ');
                    system(call->str);
                    return;
                } else {
                    cb++;
                }
            }
        printf("You don't binding with name: %s\n", c_bind);
        break;
        }
    }
}


