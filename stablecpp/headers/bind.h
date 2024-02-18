#ifndef _BIND_H_
#define _BIND_H_
#include <string>
#include <vector>
#include <list>

namespace BIND {

    typedef enum {
        BIND_OK,
        BIND_ERROR_SAVE_ON_DISK,

    } BIND_CODE;

    class Cbind {
        public:
            std::string name;
            std::string value;

            Cbind();
            Cbind(std::string name, std::string value);
            ~Cbind();

            bool operator==(Cbind& cbind);
    };

    class Bind {
        public:
            std::string name;
            std::string value;
            std::list<Cbind> children;

            Bind();
            Bind(std::string name, std::string value);
            ~Bind();
            BIND_CODE add_child(Cbind cbind);
            Cbind get_child(std::string name) const;
            BIND_CODE delete_child(std::string cname);
    };
    
    class Bindapp {
        public:
            std::list<Bind> binds;
            Bindapp();
            ~Bindapp();

            BIND_CODE add_parent(Bind bind);
            Bind get_parent(std::string name) const;
            BIND_CODE delete_parent(std::string name);
    };

    Bindapp load_config_from_disk();
    BIND_CODE save_config_on_disk(Bindapp& app);
    void show_help();
    void show_binds(const Bindapp& app);
    std::list<std::string> split_input(std::string input);

}


#endif //_BIND_H_
