#include "../headers/bind.h"
#include <unistd.h>
#include <stdio.h>
#include <iostream>





namespace BIND {


    const char *PATH_TO_CONF = "/home/%s/.local/share/bindhelper/resources/bindconfp.cfg";
    char full_path[250]; 

    Cbind::Cbind() {}

    Cbind::Cbind(std::string name, std::string value) {
        this->name = name;
        this->value = value;
    } 
    Cbind::~Cbind() {}
    bool Cbind::operator==(Cbind& cbind) {
        return this->name == cbind.name && this->value == cbind.value; 
    }


    Bind::Bind() {}
    Bind::Bind(std::string name, std::string value) {
        this->name = name;
        this->value = value;
    }

    Bind::~Bind() {}

    BIND_CODE Bind::add_child(Cbind cbind) {
        this->children.push_back(cbind);
        return BIND_OK;
    }

    Cbind Bind::get_child(std::string name) const{
        for(Cbind cb : this->children) {
            if (cb.name == name) {
                return cb;
            }
        }
        return Cbind{};
    }

    BIND_CODE Bind::delete_child(std::string name) {
        Cbind temp;
        std::list<Cbind>::iterator it = this->children.begin();
        for ( ; it != this->children.end(); it++) {
            if (it->name == name) {
                this->children.erase(it);
                break;
            }
        }
        return BIND_OK;
    }


    Bindapp::Bindapp() {}
    Bindapp::~Bindapp() {}

    BIND_CODE Bindapp::add_parent(Bind bind) {
        this->binds.push_back(bind);

        return BIND_OK;
    }

    Bind Bindapp::get_parent(std::string name) const{
        Bind b;
        for(Bind bind : this->binds) {
            if (bind.name == name) {
                b = bind;
                break;
            }
        }
        return b;
    }

    BIND_CODE Bindapp::delete_parent(std::string name) {
        Bind b;
        auto it = this->binds.begin();
        for (; it != this->binds.end(); it++) {
            if (it->name == name) {
                this->binds.erase(it);
                break;
            }
        }
        return BIND_OK;
    }


    static void set_full_path() {
        const char *name = getlogin();
        if (name == nullptr) {
            fprintf(stderr, "Cont't get login from getlogin() func\n");
            return;
        }
        
        snprintf(full_path, 250, PATH_TO_CONF, name);
    }


    Bindapp load_config_from_disk() {
        Bindapp bapp;
        set_full_path();
        FILE *f = fopen(full_path, "r");
        if (f == NULL) {
            f = fopen(full_path, "w");
            fclose(f);
            return bapp;
        }

        std::list<std::string> config;

        char buf[200];
        while(!feof(f)) {
            fgets(buf, 200, f);
            config.push_back(buf);
        }
        std::list<std::string>::iterator it = config.begin();

        std::string t = *it;
        for(int i = 0; it != config.end(); ) {
            it++;
            if (t.find_first_not_of("-p")) {
                std::string name = *(it++);
                std::string value = *(it++);
                Bind b {name, value};

                while(t.find_first_not_of("-c")) {
                    name = *(it++);
                    value = *(it++);
                    b.add_child(Cbind{name, value});
                    t = *(it++);
                }
                bapp.add_parent(b);
            }
        }

        return bapp;
    }

    BIND_CODE save_config_on_disk(Bindapp& app) {
        FILE *f = fopen(full_path, "w");
        if (f == NULL) {
            fprintf(stderr, "Can't open config file %s\n", full_path);
            return BIND_ERROR_SAVE_ON_DISK;
        }

        if (app.binds.size() == 0) {
            return BIND_OK;
        }

        for(auto& b : app.binds) {
            fputs("-p\n", f);
            fputs((b.name + "\n").c_str(), f);
            fputs((b.value + "\n").c_str(), f);

            for(auto& c : b.children) {
                fputs("-c\n", f);
                fputs((c.name + "\n").c_str(), f);
                fputs((c.value + "\n").c_str(), f);
            }
        }
        return BIND_OK;
    }

    const char *help =  "flag 'b' -----------------------------\n"
                        "b 'bindname' -> open site\n"
                        "b 'bindname 'childbindname' -> open child site'\n"                          
                        "b cp 'bindname' 'bindvalue' -> create new parent bind\n"                     
                        "b cc 'bindname' 'childbindname' 'childvalue' -> create new child bind\n"    
                        "b dp 'bindname' -> delete bind and all chilern binds\n"                      
                        "b dc 'bindname' 'childbindname' -> delete child bind\n";                      

    void show_help() {
        std::cout << help;
    }

    void show_binds(const Bindapp& app) {
        std::cout << "-------------------------------------------------------------------------------------\n";
        for(auto& b : app.binds) {
            printf("Parent bind name - %s, value - %s\n", b.name.c_str(), b.value.c_str());
            for(auto& c : b.children) {
                printf("    Child bind name - %s, value - %s\n", c.name.c_str(), c.value.c_str());
            }
            std::cout << "              <=====>";
        }
        std::cout << "-------------------------------------------------------------------------------------\n";
    }

    std::list<std::string> split_input(std::string input) {
        std::list<std::string> split;

        const char* a = input.data();
        char buf[100];
        for(int i = 0, j = 0; ; i++) {
            if (a[i] == ' ') {
                buf[j] = '\0';
                split.push_back(buf);
                j = 0;
                continue;
            }
            if (a[i] == '\n' || a[i] == '\0') {
                buf[j] = '\0';
                split.push_back(buf);
                break;
            }
            buf[j++] = a[i];
        }
        return split;
    }
}

