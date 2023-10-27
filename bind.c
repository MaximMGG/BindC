#include "util.h"


int main() {

    List list = ReadAllLineFromConfig();
    for(int i = 0; i < list.length; i++){
        printf("%s", list.line[i]);
    }

    return 0;
}
