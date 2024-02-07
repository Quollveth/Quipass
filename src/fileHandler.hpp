#ifndef FILEHANDLER_INCLUDED
#define FILEHANDLER_INCLUDED

#include <string>
#include <sstream>
#include <vector>

struct login{
    std::string login;
    std::string username;
    std::string password;
};

inline std::string loginToJson(struct login loginInfo){

    std::stringstream ret;

    ret << "{";
    ret << "\"login\":\"" << loginInfo.login << "\",";
    ret << "\"username\":\"" << loginInfo.username << "\",";
    ret << "\"password\":\"" << loginInfo.password << "\"";
    ret << "}";

    return ret.str();
}

class fileHandler {
    private:
    std::vector<struct login> storedLogins;

    public:
    fileHandler() : storedLogins(std::vector<struct login>()) {}

    struct login getLogin(int index);

    std::string getLoginJson(int index);

    std::string JsonOut();

    void addLogin(std::string &name, std::string &user, std::string &pass);
};

#endif