#include "../headers/bind.h"
#include <unistd.h>
#include <stdio.h>





namespace BIND {


    const char *PATH_TO_CONF = "/home/%s/.local/share/bindhelper/resources/bindconfp.cfg";
    char full_path[250]; 

    Cbind::Cbind() {}

    Cbind::Cbind(std::string name, std::string value) {
        this->name = name;
        this->value = value;
    } 
    Cbind::~Cbind() {}


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

    Cbind Bind::get_child(std::string name) {
        for(Cbind cb : this->children) {
            if (cb.name == name) {
                return cb;
            }
        }
        return Cbind{};
    }

    BIND_CODE Bind::delete_child(std::string name) {
        Cbind temp;
        for(Cbind cb : this->children) {
            if (cb.name == name) {
                temp = cb;
            }
        }
        this->children.remove(temp);

        return BIND_OK;
    }


    Bindapp::Bindapp() {}
    Bindapp::~Bindapp() {}

    BIND_CODE Bindapp::add_parent(Bind bind) {
        this->binds.push_back(bind);

        return BIND_OK;
    }

    Bind Bindapp::get_parent(std::string name) {
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
        for(Bind bind : this->binds) {
            if (bind.name == name) {
                b = bind;
            }
        }
        this->binds.remove(b);
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



}
