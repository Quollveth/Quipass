#include "uiHandler.hpp"

#include "webview.h"

#include <string>
#include <iostream>
#include <random>

constexpr const auto html =
R"html(
<!-- this is a copy of the html used for the gui, including css and javascript for ease of editing -->

<style>
    body {
        margin: 0;
        font-family: Arial, sans-serif;
        background-color: #e7e7e7;
    }
    .container {
        display: flex;
        flex-direction: column;
        min-height: 100vh;
    }
    #topbar {
        background-color: #333;
        color: #fff;
        padding: 10px 20px;
        display: flex;
        justify-content: space-between;
        align-items: center;
    }
    #topbar ul {
        display: flex;
        list-style-type: none;
        margin-left: -15;
        margin-right: 10;
    }
    #topbar li {
        list-style: none;
    }
    .menu-button {
        background-color: transparent;
        border: none;
        color: #fff;
        cursor: pointer;
        font-size: 16px;
        padding: 5px 10px;
        margin-left: 10px;
        transition: color 0.3s;
    }
    #button-new:hover{
        color: #00aeff;
    }
    #button-save:hover{
        color: #ff9900;
    }
    #button-exit:hover{
        color: #ff4848;
    }
    #content {
        flex: 1;
        display: flex;
        padding: 20px;
    }
    #left-section {
        width: 30%;
        background-color: #fff;
        padding: 20px;
        box-sizing: border-box;
    }
    #left-section h2 {
        margin-top: 0;
        color: #333;
    }
    #left-section ul {
        list-style-type: none;
        padding: 0;
    }
    #left-section ul li {
        margin-bottom: 10px;
    }
    #right-section {
        width: 70%;
        background-color: #fff;
        padding: 20px;
        box-sizing: border-box;
    }
    #right-section h2 {
        margin-top: 0;
        color: #333;
    }
    #login-form input[type="text"],
    #login-form input[type="password"],
    #login-form input[type="range"],
    #login-form input[type="checkbox"],
    #login-form input[type="button"] {
        width: 100%;
        padding: 10px;
        margin-bottom: 15px;
        box-sizing: border-box;
        border: 1px solid #ccc;
        border-radius: 5px;
        font-size: 16px;
    }
    #login-form input[type="button"] {
        background-color: #333;
        color: #fff;
        cursor: pointer;
        transition: background-color 0.3s, color 0.3s;
    }
    #login-form input[type="button"]:hover {
        background-color: #919191;
    }
    #password-options label {
        display: block;
        margin-bottom: 5px;
    }
    #password-options input[type="checkbox"] {
        margin-right: 5px;
    }
    #info-buttons {
        display: flex;
        justify-content: space-between;
    }
    #info-buttons div {
        display: flex;
    }
    #info-buttons input[type="button"] {
        min-width: 100;
        max-width: 100;
        max-height: 40;
    }
    #list-header input{
        margin-top: 5;
    }
    .separator {
        border-top: 1px solid #ccc; /* Specify the thickness, style, and color of the separator */
        margin: 20px 0; /* Adjust margin as needed */
    }
    .disabled {
        background-color: #919191 !important;
    }
</style>

<div class="container">
    <div id="topbar">
        <button id="button-new" class="menu-button">New Login</button>
        <ul>
            <li><button id="button-save" class="menu-button">Save</button></li>
            <li><button id="button-exit" class="menu-button">Exit</button></li>
        </ul>
    </div>

    <div id="content">
        <div id="left-section">
            <div id="list-header">           
                <input type="file" id="fileInput" accept=".txt">
                <input id="file-password" type="password" name="password" placeholder="File Password">
                <input type="button" value="decrypt" id="decrypt-button">
            </div>
            <div class="separator"></div>
            <h2>Saved Logins</h2>     
            <ul id="logins-list">

            </ul>
        </div>

        <div id="right-section">
            
        </div>
    </div>
</div>

<script>
    //webview doesn't have console.log lol
    function printOut(string){
        window.printOut(string);
    }
    // screen when adding a new login
        const newLoginSection = `
        <h2 id="main-header">New Login</h2>
        <div id="login-form">
            <input class="addNewText" id="login-name-field" type="text" name="login" placeholder="Login">
            <input class="addNewText" id="username-field" type="text" name="username" placeholder="Username">
            <input class="addNewText" id="password-field" type="text" name="password" placeholder="Password">
            <input class="addNewButton" id="generate-password" type="button" value="Generate">
            <div id="password-options">
                <label id="length-label" for="length-slider">Length: 12</label>
                <input id="length-slider" type="range" id="length-slider" min="8" max="36" value="12" step="1">

                <label for="uppercase-check">Uppcase</label>
                <input class="addNewCheck" id="1" type="checkbox" name="uppercase-check" checked>

                <label for="lowercase-check">Lowercase</label>
                <input class="addNewCheck" id="2" type="checkbox" name="lowercase-check" checked>

                <label for="numbers-check">Numbers</label>
                <input class="addNewCheck" id="4" type="checkbox" name="numbers-check" checked>

                <label for="symbol-check">Symbols</label>
                <input class="addNewCheck" id="8" type="checkbox" name="symbol-check" checked>
            </div>
            <input class="addNewButton" id="save-button" type="button" value="Save">
        </div>
        `;
    // screen when viewing an existing login
    const displayLoginSection = `
        <h2 id="main-header">Saved login</h2>
        <div id="login-form">
            <input id="login-name" type="text" name="login" placeholder="Login" disabled>
            <input id="username" type="text" name="username" placeholder="Username" disabled>
            <input id="password" type="text" name="password" placeholder="Password" disabled>
            <div id="info-buttons">
                <div>
                    <input type="button" value="show/hide" id="show-button">
                    <input type="button" value="copy" id="copy-button">
                </div>
                <input type="button" value="delete" id="delete-button">
            </div>
        </div>
    `;
    //

    //menu stuff
    var menuButtons = [
        document.getElementById('button-new'),
        document.getElementById('button-save'),
        document.getElementById('button-exit')
    ];

    menuButtons.forEach((element, index)=>{
        element.addEventListener('click', ()=>{
            window.menuButton(index);
        });
    });

    //file loading stuff
    var fileIn = document.getElementById('fileInput');
    var decryptBtn = document.getElementById('decrypt-button');
    var passwordField = document.getElementById('file-password');
    var currFile;
    
    fileIn.addEventListener('change', (event)=>{
        currFile = event.target.files[0];
    });

    decryptBtn.addEventListener('click', ()=> {
        const reader = new FileReader();
        reader.onload = function(event){
            const contents = event.target.result;
            var fileInfo = {
                name: currFile.name,
                size: currFile.size,
                password: passwordField.value,
                content: contents
            };
            window.sendFile(fileInfo);
        }
        reader.readAsBinaryString(currFile);
    });

    //screen toggle stuff
    var addingLogin = false;
    var rightSection = document.getElementById('right-section');

    function toggleScreen(){
        addingLogin = !addingLogin;
        if(addingLogin){
            rightSection.innerHTML = newLoginSection;
            loadAddLoginScreen();
        } else {
            rightSection.innerHTML = displayLoginSection;
        }
    }

    //add new login stuff
    var generatePasswordBtn;
    var saveLoginBtn;
    var newLoginTexts = [];
    var newLoginChecks = [];
    var newLoginSlider;
    var newLoginLength;
    var passwordOptions = 15;
    var passwordLength = 12;

    function loadAddLoginScreen(){
        //dom has to be reloaded every time we change it since the elements that don't exist can't be referenced
        generatePasswordBtn = document.getElementById('generate-password');
        saveLoginBtn = document.getElementById('save-button');

        newLoginFields = document.querySelectorAll('.addNewText');
        newLoginChecks = document.querySelectorAll('.addNewCheck');
        newLoginSlider = document.getElementById('length-slider');
        newLoginLength = document.getElementById('length-label');

        newLoginSlider.addEventListener('change',(element)=>{
            passwordLength = element.target.value;
            newLoginLength.innerHTML = `Length: ${passwordLength}`;
        });

        newLoginChecks.forEach((element)=>{
            element.addEventListener('change',()=>{
                if(element.checked){
                    passwordOptions += parseInt(element.id);
                }
                else {
                    passwordOptions -= parseInt(element.id);
                }
                if(passwordOptions == 0){
                    generatePasswordBtn.classList.add("disabled");
                    generatePasswordBtn.value = 'Select at least one option';
                }
                else {
                    generatePasswordBtn.classList.remove("disabled");
                    generatePasswordBtn.value = 'Generate';
                }
            });
        });

        generatePasswordBtn.addEventListener('click', ()=> {
            if(passwordOptions == 0){
                return;
            }
            const passwordInput = {
                length: passwordLength,
                flags: passwordOptions
            };
            window.generatePassword(JSON.stringify(passwordInput)).then(result=>{
                newLoginFields[2].value = result;
            });
        });
    }

    toggleScreen();
</script>
)html";

int randomInRange(int min, int max, bool newSeed = true) {
    static std::mt19937 eng{std::random_device{}()};
    if(newSeed){
        eng.seed(std::random_device{}());
    }
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(eng);
}

enum passwordFlags {
    NONE   = 0,
    UPPER  = 1,         // 0001 : 1
    LOWER  = 1 << 1,    // 0010 : 2
    NUMBER = 1 << 2,    // 0100 : 4
    SYMBOL = 1 << 3     // 1000 : 8
};

inline char randomCharacter(enum passwordFlags type){
    switch(type){
        case SYMBOL: //possible returns: # $ & '(gets turned into @ since @ is all the way in ascii 64 for some reason)
            {
                char ret = randomInRange(35,39); 
                return (ret == 39)?'@':static_cast<char>(ret);
            }
        case NUMBER:
            return randomInRange(48,57);
        case UPPER:
            return randomInRange(65,90);
        case LOWER:
            return randomInRange(97,122);
    }
    return 0;
}

std::string generatePassword(int length, int flags){
    enum passwordFlags type;
    int choice;
    std::string password = "";
    for(int i=0;i<length;i++){
        type = NONE;
        while(type == NONE){
            choice = randomInRange(1,4,false);
            switch(choice){
                case 1: 
                    if(flags & UPPER) type = UPPER;
                    break;
                case 2:
                    if(flags & LOWER) type = LOWER;
                    break;
                case 3:
                    if(flags & NUMBER) type = NUMBER;
                    break;
                case 4:
                    if(flags & SYMBOL) type = SYMBOL;
                    break;            
                }
        }
        password += randomCharacter(type);
    }
    return password;
}

std::string stringToResponse(const std::string& input) {
    return "[\"" + input + "\"]";
}

void initializeUI(webview::webview *w){
    w->set_title("Password Manager");
    w->set_size(480, 320, WEBVIEW_HINT_NONE);

    //binds
        // seq -> sequential id of the request
        // req -> json array with all arguments passed
        // resolve returns the function (resolved promise), resolve(request id, status, return value)
    w->bind(
        "printOut",
        [&](const std::string &seq, const std::string &req, void *){
            std::cout << req << std::endl;
        },
        nullptr
    );

    w->bind(
        "menuButton",
        [&](const std::string &seq, const std::string &req, void *){
            std::cout << req[1] << std::endl;
            if(std::atoi(&req[1]) == 0){
                std::cout << "new login pressed" << std::endl;
            }
        },
        nullptr
    );
    w->bind(
      "sendFile",
      [&](const std::string &seq, const std::string &req, void *) {
        std::cout << req << std::endl;
      },
      nullptr
    );
    w->bind(
        "generatePassword",
        [&](const std::string &seq, const std::string &req, void *) {

            std::string innerJson = webview::detail::json_parse(req, "", 0);
            int length = std::stoi(webview::detail::json_parse(innerJson, "length", 0));
            int flags =  std::stoi(webview::detail::json_parse(innerJson, "flags", 0));
            
            std::string password = generatePassword(length,flags);

            w->resolve(seq,0,stringToResponse(password));
        },
        nullptr
    );

    w->set_html(html);
    w->run();
}