#include <util.h>


typedef struct {
    Bind bind;
    char *dir;
} User;

typedef struct {
    char line[200];
} cfgList;

#define PATH_TO_CONF "resources/config.xml"
#define PATH_TO_TRANSLATOR "https://translate.google.com/?hl=ru&sl=en&tl=ru&text=%s\%0A&op=translate"


bool equals(char *str1, char *str2) {
    for(int i = 0; i < sizeof(str1) / sizeof(str1[0]); i++){
        if (str1[i] == str2[i]) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}

void initialise() {
    User user;

    FILE *f = fopen(PATH_TO_CONF, "r");
    char str[100];

    while(feof(f) != 0){
       fread(str, 100, 1, f); 
       if (equals(str, "<PathToDir>")) {
            fread(str, 100, 1, f);
            user.dir = str;
       }
       if (equals(str, "<Bind>")) {
            
       }
    }
}











void test() {
    FILE *f = fopen(PATH_TO_CONF, "r");
    cfgList *list;
    list = malloc(sizeof(list->line) * 100);
    int p = 0;
    while(!feof(f)) {
        list += p;
        fgets(list->line, 200, f);
        printf("%s\n", list->line);
        p++;
    }

    fclose(f);

    // for(int i = 0; i < 30; i++){
    //    list += i;
    //    printf("%s\n", list->line);
    // }
}






int main() {

    test();
    // printf("%d\n", equals("Hello", "Hello"));
    return 0;
}
