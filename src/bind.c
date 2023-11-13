#include "../headers/util.h"


void ShowHelp();
User * InitUser();
int mapUserCommand(char **);

int main() {

    // puts("Hello let's start or enter help for show some referencies");
    User *u = InitUser();
   
    char *user_enter;
    fgets(user_enter, 100, stdin);

    while(!(compare_string(user_enter, "exit"))) {

        List *user_command = parsUserEnter(user_enter, strlen(user_enter));

        if (compare_string(user_enter, "show")) {
            ShowHelp();
        }
        char b = 2;

        //TODO (Maxim) think about switch statement for user_command array
        switch (user_command->line[0][0]) {
            case 't' : {
                        showTranslation(user_command->line[1]);
                        break;
                       }
            case 'a' : {
                        appentWord(u, user_command->line[1]);
                       }
            case 'f' : {
                        switch(user_command->line[1][0]) {
                            case 's' : {
                                if(!setCurrentFile(user_command->line[2], u)) {
                                    fprintf(stderr, "ERROR: You don't have file %s\n", user_command->line[2]);
                                    break;
                                }
                                       }
                            case 'c' : {
                                createNewFile(u, user_command->line[2]);
                                break;
                                       }
                            case 'd' : {
                                setUserDir(u, user_command->line[2]);
                                       }
                            if (!strcmp(user_command->line[1], "show")) {
                                showAllFilesInDir(u);
                                break;
                            }
                        }
                       }
            case 'b' : {

                       }
            case 'c' : {

                       }
        }
      

        fgets(user_enter, 100, stdin);
    }

    return 0;
}


void ShowHelp() {
    puts("t word -> go to google tranlator and translate \"word\"");
    puts("a word - translation -> add to current file word and translation");
    puts("f:");
    puts("\tf s file_name -> set current file");
    puts("\tf c file_name -> create new file");
    puts("\tf d full_path_to_dir -> set full path to directory with files");
    puts("\tf show -> show all file in directory");
    puts("b:");
    puts("\tb cp name value -> create bind with name (name) and http(s) (value)");
    puts("\tb cc parent_name name value -> create child of bind (parent_name) with name (name) and http(s) (value)");
    puts("\tb dp name -> delete bind with name (name)");
    puts("\tb dc parent_name name -> create child of bind (parent_name) with name (name)");
    puts("\tb show -> show all binds");
    puts("c:");
}



User * InitUser() {
    List *list = ReadAllLineFromConfig();
    User *u = malloc(sizeof(*u));
    u->dir = get_str_between(list->line[0], '>', '<');
    set_binds_from_config(list, u);
    return u;
}


