#include "../headers/bind.h"
#include <stdio.h>
#include <iostream>



int main() {

    BIND::Bindapp app = BIND::load_config_from_disk();

    char buf[200];
    fgets(buf, 200, stdin);
    std::string sb = buf;

    if (sb == "help") {
        BIND::show_help();
        fgets(buf, 200, stdin);
        sb = buf;
    }

    while(sb != "exit") {
        std::list<std::string> input = BIND::split_input(sb);
        auto it = input.begin();
        it++;
        if (*it == "cp") {
            if (input.size() < 4) {
                fprintf(stderr, "Invalid parameter\n");
                goto next;
            } else {
                it++;
                std::string name = *(it++);
                std::string value = *(it++);
                app.add_parent(BIND::Bind(name, value));
                std::cout << "Bind successfully added\n";
                goto next;
            }
        } else if (*it == "cc") {
            if (input.size() < 5) {
                fprintf(stderr, "Invalid parameter\n");
                goto next;
            } else {
                it++;
                BIND::Bind temp = app.get_parent(*(it++));
                std::string name = *(it++);
                std::string value = *it;
                temp.add_child(BIND::Cbind{name, value});
                std::cout << "Child bind successfully added\n";
                goto next;
            }
        } else if (*it == "dp") {
            if (input.size() < 3) {
                fprintf(stderr, "Invalid parameter\n");
                goto next;
            } else {
                it++;
                app.delete_parent(*it);
                std::cout << "Bind successfully deleted\n";
                goto next;
            }
        } else if (*it == "dc") {
            if (input.size() < 4) {
                fprintf(stderr, "Invalid parameter\n");
                goto next;
            } else {
                it++;
                BIND::Bind temp = app.get_parent(*it);
            }
        }

    next:
        fgets(buf, 200, stdin);
    }




    return 0;
}
