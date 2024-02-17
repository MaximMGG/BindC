#ifndef _BIND_H_
#define _BIND_H_
#include <string>
#include <vector>
#include <list>

namespace BIND {

    typedef enum {
        BIND_OK,

    } BIND_CODE;

    class Cbind {
        public:
            std::string name;
            std::string value;

            Cbind();
            Cbind(std::string name, std::string value);
            ~Cbind();
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
            Cbind get_child(std::string name);
            BIND_CODE delete_child(std::string cname);
    };
    
    class Bindapp {
        public:
            std::list<Bind> binds;
            Bindapp();
            ~Bindapp();

            BIND_CODE add_parent(Bind bind);
            Bind get_parent(std::string name);
            BIND_CODE delete_parent(std::string name);
    };

    Bindapp load_config_from_disk();

}


#endif //_BIND_H_
