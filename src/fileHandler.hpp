#ifndef FILEHANDLER_INCLUDED
#define FILEHANDLER_INCLUDED

#include "webview.h"
#include "base64.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct login{
    std::string login;
    std::string username;
    std::string password;
};

#define SIGNATURE "QUIPASSFILE"

enum loginFields{
    NAME,
    USER,
    PASS
};

inline std::string loginToJson(const struct login& loginInfo){

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

    void parseLoginJson(std::string jsonData);

    public:
    int logins;

    fileHandler() : storedLogins(std::vector<struct login>()), logins(0) {}

    struct login getLogin(int index);

    std::string getLoginJson(const login& loginInfo);
    std::string getLoginJson(int index);

    std::string JsonOut();

    std::string getLoginNames();
    
    void addLogin(const struct login& loginInfo);

    void updateLogin(int index,enum loginFields field,const std::string &newValue);

    std::string exportFile(const std::string &filePassword);

    bool importFile(const std::string &fileData, const std::string &filepassword);
};

#endif