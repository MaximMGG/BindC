#include "util.h"


int main() {

    puts("Hello let's start or enter help for show some referencies");
   
    char *user_enter;
    gets_s(user_enter, 100);

    while(!strcmp(user_enter, "exit")) {

        char **user_command = parsUserEnter(user_enter, strlen(user_enter));

        //TODO (Maxim) think about switch statement for user_command array


        gets_s(user_enter, 100);
    }


    return 0;
}
