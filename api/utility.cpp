#include <string>

#include "utility.hpp"

std::string get_fname_from_path(char* filepath){
    std::string str(filepath);

    size_t found = str.rfind("/");
    //if it can be found crop string
    if (found != std::string::npos){
        return str.substr(found + 1); //start from next char
    }

    return str;
}
