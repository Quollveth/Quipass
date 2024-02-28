#include "fileHandler.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

struct login fileHandler::getLogin(int index){
    auto it = storedLogins.begin();
    std::advance(it, index);
    return *it;
}    

std::string fileHandler::getLoginJson(int index){
    if(index < 0 || index > storedLogins.size()){
        return "{}";
    }
    auto it = storedLogins.begin();
    std::advance(it, index);
    const login& loginInfo = *it;

    return loginToJson(loginInfo);
}

std::string fileHandler::JsonOut() {
    std::stringstream json;
    json << "[";
    bool first = true;
    for (const auto& login : storedLogins) {
        if (!first) {
            json << ",";
        }
        json << loginToJson(login);
        first = false;
    }
    json << "]";
    return json.str();
}

std::string fileHandler::getLoginNames(){
    std::string json = "[";
    auto it = storedLogins.begin();
    while(it != storedLogins.end()){
        json += "\"";
        json += storedLogins.at(std::distance(storedLogins.begin(), it)).login;
        json += "\"";
        ++it;
        if(it != storedLogins.end()){
            json += ",";
        }
    }
    return json + "]";
}

void fileHandler::addLogin(const struct login& loginInfo){
    storedLogins.push_back(loginInfo);
}

void fileHandler::updateLogin(int index,enum loginFields field,const std::string &newValue){
    switch (field)
    {
    case NAME:
        storedLogins[index].login = newValue;
        break;

    case USER:
        storedLogins[index].username = newValue;
        break;
    
    case PASS:
        storedLogins[index].password = newValue;
        break;
    
    default:
        break;
    }
}

std::string fileHandler::exportFile(const std::string &filePassword){   

    return base64_encode(JsonOut().insert(0,SIGNATURE));
}

bool fileHandler::importFile(const std::string &fileData, const std::string &filePassword){
    
    std::string loginData = base64_decode(fileData);

    if(loginData.substr(0,11).compare(SIGNATURE)!=0){
        return false;
    }

    loginData.erase(0,11);
    parseLoginJson(loginData);
    return true;
}

void fileHandler::parseLoginJson(std::string jsonData){
    int i = 0;
    while(true){
        std::string current = webview::detail::json_parse(jsonData, "", i);
        if(current.empty()) break;

        struct login newLogin;
        newLogin.login = webview::detail::json_parse(current, "login", 0);
        newLogin.username = webview::detail::json_parse(current, "username", 0);
        newLogin.password = webview::detail::json_parse(current, "password", 0);

        addLogin(newLogin);

        i++;
    }
}