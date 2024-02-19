#include "../headers/bind.h"
#include "../headers/util.h"
#include <stdio.h>


void end_app(BIND_APP *app);
void execute(str *e);

int main() {
    BIND_APP *app = Bind_load_binds_from_disk();
    char buf[200];
    fgets(buf, 200, stdin);

    if(strcmp(buf, "help") == 0) {
        Bind_show_help();
        fgets(buf, 200, stdin);
    }


    char *sbuf = NULL;
    char **flags = NULL;
    int flags_count = 0;
    BIND *temp = NULL;
    int err_code = 0;

    while (strcmp(buf, "exit\n") != 0) {
        flags = string_split(sbuf, ' ');
        if (flags_count < 2) {
            printf("Do not have any option with this command %s\n", buf);
            goto next_cycle;
        }
        if (strcmp(flags[1]->str, "cp") == 0) {
            if (flags_count == 4) {
                err_code = Bind_add_parent(Bind_create_bind(flags[2], flags[3]), app);
                if (!err_code) {
                    printf("Bind successfully added\n");
                }
            }
        } else if (strcmp(flags[1]->str, "cc") == 0) {
            if (flags_count == 5) {
                temp = Bind_get_bind_byname(app, flags[2]);
                if (temp == NULL) {
                    fprintf(stderr, "Do not have bind with name %s \n", flags[2]->str);
                    goto next_cycle;
                }
                err_code = Bind_add_child(temp, flags[3], flags[4]);
                if (!err_code) {
                    printf("Child bind successfully added\n");
                } else {
                    printf("%s\n", Bind_get_error());
                    goto next_cycle;
                }
            } else {
                printf("Thomething miss, try agane\n");
                goto next_cycle;
            }
        } else if (strcmp(flags[1]->str, "dp") == 0) {
            if (flags_count == 3) {
                err_code = Bind_delete_bind(flags[2], app);
                if (!err_code) {
                    printf("Bind successfully deleted\n");
                    goto next_cycle;
                } else {
                    printf("%s\n", Bind_get_error());
                    goto next_cycle;
                }
            } else {
                printf("Thomething miss, try agane\n");
                goto next_cycle;
            }
        } else if (strcmp(flags[1]->str, "dc") == 0) {
            if (flags_count == 4) {
                temp = Bind_get_bind_byname(app, flags[2]);
                if (temp == NULL) {
                    fprintf(stderr, "Do not have bind with name %s \n", flags[3]->str);
                    goto next_cycle;
                }
                err_code = Bind_remove_child(temp, flags[3]);
                if (!err_code) {
                    printf("Child bind successfully deleted\n");
                    goto next_cycle;
                } else {
                    printf("%s\n", Bind_get_error());
                    goto next_cycle;
                }
            } else {
                printf("Thomething miss, try agane\n");
                goto next_cycle;
            }
        } else {
            if (flags_count == 2) {
                if (strcmp(flags[1]->str, "show") == 0) {
                    Bind_show_binds(app);
                    goto next_cycle;
                }
                execute(Bind_get_bindval(flags[1], app));
                goto next_cycle;
            } else if (flags_count == 3) {
                execute(Bind_get_bindchildval(flags[1], flags[2],app));
            }
        }

                
next_cycle:
        str_free(sbuf);
        for(int i = 0; i < flags_count; i++) {
            str_free(flags[i]);
        }
        free(flags);
        fgets(buf, 200, stdin);
    }

    end_app(app);

    return 0;
}

void end_app(BIND_APP *app) {
    int err_code = Bind_save_binds_on_disk(app);
    Bind_destroy_app(app);
    if (!err_code) {
        printf("Bye\n");
    } else {
        fprintf(stderr, "Thomething went wrong, binds didn't save\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void execute(str *e) {
    char buf[250];
    char *fmt = "google-chrome %s";
    if (e == NULL) {
        fprintf(stderr, "Don't have bind with that name\n");
    } else {
        snprintf(buf, 250, fmt, e->str);
        system(buf);
    }
}
