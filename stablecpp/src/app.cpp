#include "../headers/bind.h"
#include <stdio.h>
#include <iostream>


void execute(std::string bind);


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

    while(sb != "exit\n") {
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
                BIND::Bind *temp = app.get_parent(*(it++));
                std::string name = *(it++);
                std::string value = *it;
                temp->add_child(BIND::Cbind{name, value});
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
                BIND::Bind* temp = app.get_parent(*(it++));
                temp->delete_child(*it);
                std::cout << "Child bind successfully deleted\n";
                goto next;
            }
        } else if (*it == "show") {
                BIND::show_binds(app);
                goto next;
        } else {
            if (input.size() == 2) {
                BIND::Bind* temp = app.get_parent(*it);
                if (temp->name.size() < 1) {
                    fprintf(stderr, "Don't have bind with name %s\n", it->c_str());
                    goto next;
                }
                execute(temp->value);
                goto next;
            } else if (input.size() == 3) {
                BIND::Bind *btemp = app.get_parent(*(it++));
                if (btemp->name.size() < 1) {
                    fprintf(stderr, "Don't have bind with name %s\n", it->c_str());
                    goto next;
                }
                BIND::Cbind ctemp = btemp->get_child(*it);
                if (ctemp.name.size() < 1) {
                    fprintf(stderr, "Don't have child bind with name %s\n", it->c_str());
                    goto next;
                }
                execute(ctemp.value);
                goto next;
            } else {
                fprintf(stderr, "Invalid parameters\n");
            }
        }
    next:
        fgets(buf, 200, stdin);
        sb = buf;
    }

    BIND::BIND_CODE code = BIND::save_config_on_disk(app);
    if (code == BIND::BIND_OK) {
        std::cout << "Bye\n";
    } else {
        std::cout << "Thomething went wrong, bind do not save\n";
    }
    return 0;
}

void execute(std::string bind) {
    std::string executor {"google-chrome "};
    executor += bind;
    system(executor.c_str());
}



