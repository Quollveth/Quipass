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

    std::stringstream ret;
    ret << "{";
    ret << "\"login\":\"" << loginInfo.login << "\",";
    ret << "\"username\":\"" << loginInfo.username << "\",";
    ret << "\"password\":\"" << loginInfo.password << "\"";
    ret << "}";

    return ret.str();
}

std::string fileHandler::JsonOut(){
    std::string json = "[";
    auto it = storedLogins.begin();
    while (it != storedLogins.end()) {
        json += getLoginJson(std::distance(storedLogins.begin(), it));
        ++it;
        if (it != storedLogins.end()) {
            json += ",";
        }
    }
    return json + "]";
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