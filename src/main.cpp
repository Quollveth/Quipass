#include "webview.h"

#include "fileHandler.hpp"

#include <string>
#include <iostream>
#include <random>

constexpr const auto html = 
R"html(<!-- this is a copy of the html used for the gui, including css and javascript for ease of editing -->

<style>
    /* global styles */
    body {
        margin: 0;
        font-family: Arial, sans-serif;
        background-color: #e7e7e7;
    }

    #container { /* webview does not use head and body tags, the container div is serving as the body tag for convenience */
        display: flex;
        flex-direction: column;
        min-height: 100vh;
    }

    .separator {
        border-top: 1px solid #ccc; /* Specify the thickness, style, and color of the separator */
        margin: 20px 0;
    }

    .disabled {
        background-color: #919191 !important;
    }

    .hidden {
        display: none !important;
    }


    /* header */
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

    /* #content holds both sections */
    #content {
        flex: 1;
        display: flex;
        padding: 20px;
    }

    /* left section holds login list and file input */
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

    #file-password{
        margin-bottom: 15px;
        box-sizing: border-box;
        border: 1px solid #ccc;
        border-radius: 5px;
        height: 2em;
    }
    #fileInput{
        display: none;
    }
    #fileInputLabel{
        border: 1px solid #ccc;
        display: inline-block;
        padding: 6px 12px;
        cursor: pointer;
        background-color: #f9f9f9;
        border-radius: 4px;
    }
    #decrypt-button{
        background-color: transparent;
        border: 1px solid #ccc;
        border-radius: 5px;
        cursor:pointer;
        transition: background-color 0.3s, color 0.3s;
        height: 2em;
    }
    #decrypt-button:hover{
        background-color: #ccc;
    }
    #selectedFileLabel {
        display: inline-block;
        margin-left: 10px;
        color: #333;
    }

    .loginListItem {
        width: fit-content;
        padding-right: 30px;
        padding: 10px;
        cursor: pointer;
        border: none;
        background-color: transparent;
        font-size: 1.2em;
        text-align: start;
        color: #333;
    }
    .loginListItem:hover {
        color:#00aeff;
    }

    /* right section can switch between 3 screens, either adding a new login or showing the info of a saved login, with the option of neither */

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

    #right-section input[type="button"] {
        background-color: #333;
        color: #fff;
        font-size: 16px;
        border: 1px solid #ccc;
        cursor: pointer;
        transition: background-color 0.3s, color 0.3s;
    }
    #right-section input[type="button"]:hover {
        background-color: #919191;
    }

    /* new login screen */

    #login-form input {
        width: 100%;
        padding: 10px;
        margin-bottom: 15px;
        box-sizing: border-box;
        border: 1px solid #ccc;
        border-radius: 5px;
        font-size: 16px;
    }

    #password-options label {
        display: block;
        margin-bottom: 5px;
    }
    #password-options input[type="checkbox"] {
        margin-right: 5px;
    }

    /* view login info screen */

    #info-buttons {
        display: flex;
        justify-content: space-between;
    }
    #info-buttons div {
        display: flex;
    }
    #info-buttons input[type="button"] {
        min-width: 40;
        max-height: 40;
        font-size: small;
        margin-bottom: 5em;
    }

    #list-header input{
        margin-top: 5;
    }

    #login-delete-button{
        max-width: 20em;
    }

    #login-delete-button:hover{
        background-color: #dc3545 !important;
    }
    #deletionLabel{
        display: inline-block;
        margin-left: 10px;
        color: #333;
        width: fit-content;
        position: absolute;
        margin-top: -20px;
    }
    #button-container {
        position: relative;
    }


</style>

<div id="container">
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
                <div>
                    <input type="file" id="fileInput" accept=".txt">
                    <label for="fileInput" id="fileInputLabel"> Browse </label>
                    <span id="selectedFileLabel"></span>
                </div>
                <input id="file-password" type="password" name="password" placeholder="File Password">
                <input type="button" value="Decrypt" id="decrypt-button">
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
    // screen when adding a new login
    const newLoginSection = `
        <h2 id="main-header">New Login</h2>
        <div id="login-form">
            <input class="addNewText" id="login-name-field" type="text" name="login" placeholder="Login">
            <input class="addNewText" id="username-field" type="text" name="username" placeholder="Username">
            <input class="addNewText" id="password-field" type="text" name="password" placeholder="Password">
            <input id="generate-password" type="button" value="Generate">
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
            <input id="save-button" type="button" value="Save">
        </div>
        `;
    // screen when viewing an existing login
    const displayLoginSection = `
        <h2 id="main-header">Saved login</h2>
        <div id="login-form">
            <input class="login-info-field" id="login-info-name" type="text" name="login" placeholder="Login" disabled>
            <input class="login-info-field" id="login-info-username" type="text" name="username" placeholder="Username" disabled>
            <input class="login-info-field" id="login-info-password" type="password" name="password" placeholder="Password" disabled>
            <div id="info-buttons">
                <div>
                    <input type="button" value="Show" id="show-button">
                    <input type="button" value="Copy" id="copy-button">
                </div>
            </div>
            
            <div id="button-container">
                <label for="login-delete-button" id="deletionLabel" class="hidden">Press again to confirm</label>
                <input id="login-delete-button" type="button" value="Delete">
            </div>
        </div>
    `;

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
    var filenameLabel = document.getElementById('selectedFileLabel');
    var currFile;
    
    fileIn.addEventListener('change', (event)=>{
        currFile = event.target.files[0];
        filenameLabel.textContent = event.target.files[0].name;
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
    var currentScreen = 0;
    // 1 - new login
    // 2 - view login
    // 0 - welcome

    var rightSection = document.getElementById('right-section');

    function openNewLoginScreen(){
        currentScreen = 1;
        toggleScreen();
    }

    function openLoginInfoScreen(){
        currentScreen = 2;
        toggleScreen();
    }

    function closeScreens(){
        //closes either one of the screens opened above
        currentScreen = 0;
        toggleScreen();
    }

    function toggleScreen(){
        switch(currentScreen){
            case 0:
                rightSection.innerHTML = "";
                break;
            case 1:
                rightSection.innerHTML = newLoginSection;
                loadAddLoginScreen();
                break;
            case 2:
                rightSection.innerHTML = displayLoginSection;
                loadLoginInfoScreen();
                break;
        }

    }


    //saved logins list stuff
    var loginListHead = document.getElementById('logins-list');

    function updateList(stringValues){
        //update the list head to have a button for each saved login
        const values = JSON.parse(stringValues);

        loginListHead.innerHTML = "";
        values.forEach((value,i) => {
            const listElement = document.createElement('li');
            const button = document.createElement('button');
            button.textContent = value;
            button.id = `login-button-${i}`;
            button.addEventListener('click',(event)=>{
                window.openLogin(event.target.id);
            });
            button.classList.add('loginListItem');
            listElement.appendChild(button);
            loginListHead.appendChild(listElement);
        });
    }


    //login info stuff
    var loginInfoFields = [];
    var loginInfoShowButton;
    var loginInfoCopyButton;
    var loginInfoEditButton;
    var loginInfoDeleteButton;
    var loginInfoDeleteLabel;
    var showingPassword;
    var clickcount;

    function loadLoginInfoScreen(loginInfo){
        //dom has to be reloaded every time we change it since the elements that don't exist can't be referenced
        loginInfoShowButton = document.getElementById('show-button');
        loginInfoCopyButton = document.getElementById('copy-button');
        loginInfoEditButton = document.getElementById('login-edit-button');
        loginInfoDeleteButton = document.getElementById('login-delete-button');
        loginInfoDeleteLabel = document.getElementById('deletionLabel');

        loginInfoFields = document.querySelectorAll('.login-info-field');

        showingPassword = false;
        clickcount = 0;

        //const loginData = JSON.parse(loginInfo);

        loginInfoFields[0].value = 'loginData.login';
        loginInfoFields[1].value = 'loginData.username';
        loginInfoFields[2].value = 'loginData.password';

        document.addEventListener('click', (event) => {
            if (event.target != loginInfoDeleteButton) {
                clickcount = 0;
                loginInfoDeleteLabel.classList.add('hidden');
            }
        });

        loginInfoShowButton.addEventListener('click',()=>{
            if(showingPassword){
                loginInfoShowButton.value = 'Show';
                loginInfoFields[2].setAttribute('type','password');

            } else{
                loginInfoShowButton.value = 'Hide';
                loginInfoFields[2].setAttribute('type','text');
            }
            showingPassword = !showingPassword;            
        });

        loginInfoDeleteButton.addEventListener('click',()=>{
            clickcount++;
            if(clickcount == 1){
                loginInfoDeleteLabel.classList.remove('hidden');
                return;
            }
            if(clickcount == 2){
                loginInfoDeleteLabel.classList.add('hidden');
                clickcount = 0;
                window.deleteLogin();
                currentScreen = 0;
                toggleScreen();
            }
        });

        loginInfoCopyButton.addEventListener('click',()=>{
            navigator.clipboard.writeText(loginInfoFields[2].value);
            loginInfoCopyButton.value = 'Copied';
            setTimeout(()=>{
                loginInfoCopyButton.value = 'Copy';
            },2000)
        })
                
    }

    //add new login stuff
    var generatePasswordBtn;
    var saveLoginBtn;
    var newLoginFields = [];
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

        function refreshLogin(){
            newLoginFields.forEach(field => {
                field.value = '';
            });
            newLoginChecks.forEach(check => {
                check.checked = true;
            });
            newLoginSlider.value = 12;
            newLoginLength.innerHTML = `Length: ${passwordLength}`;
        };

        newLoginSlider.addEventListener('change',(element) => {
            passwordLength = element.target.value;
            newLoginLength.innerHTML = `Length: ${passwordLength}`;
        });

        newLoginChecks.forEach((element) => {
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
                window.updateField(2,newLoginFields[2].value);
            });
        });

        newLoginFields.forEach((element,index) => {
            element.addEventListener('change',(event)=>{
                window.updateField(index,event.target.value);
            });
        });

        saveLoginBtn.addEventListener('click', ()=>{
            window.saveLogin().then(result => {
                refreshLogin();
            });
        });
    }

    //end

    openLoginInfoScreen();
</script>)html";

//password generation stuff
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

//makes a string into a json array webview will accept
inline std::string stringToResponse(const std::string& input) {
    return "[\"" + input + "\"]";
}

int main(){
    webview::webview w(false, nullptr);

    std::string newLoginName;
    std::string newLoginUser;
    std::string newLoginPass;

    fileHandler storage;

    w.set_title("Password Manager");
    w.set_size(480, 320, WEBVIEW_HINT_NONE);

    //binds
        // seq -> sequential id of the request
        // req -> json array with all arguments passed
        // resolve returns the function (resolved promise), resolve(request id, status, return value)

    w.bind(
        "menuButton",
        [&](const std::string &seq, const std::string &req, void *){

        },
        nullptr
    );
    w.bind(
      "sendFile",
      [&](const std::string &seq, const std::string &req, void *) {
        
      },
      nullptr
    );
    w.bind(
        "generatePassword",
        [&](const std::string &seq, const std::string &req, void *) {

            std::string innerJson = webview::detail::json_parse(req, "", 0);
            int length = std::stoi(webview::detail::json_parse(innerJson, "length", 0));
            int flags =  std::stoi(webview::detail::json_parse(innerJson, "flags", 0));
            {
            std::string password = generatePassword(length,flags);

            w.resolve(seq,0,stringToResponse(password));
            }
        },
        nullptr
    );
    w.bind(
        "saveLogin",
        [&](const std::string &seq, const std::string &req, void *) {

            struct login savedLogin;

            savedLogin.login = newLoginName;
            savedLogin.username = newLoginUser;
            savedLogin.password = newLoginPass;

            storage.addLogin(savedLogin);

            //calls the updateList() function in javascript passing the login names as parameter so the list can be updated in html
            std::string ret = "updateList('" + storage.getLoginNames() + "')";
            
            w.eval(ret);

            w.resolve(seq,0,"");
        },
        nullptr
    );
    w.bind(
        "updateField",
        [&](const std::string &seq, const std::string &req, void *){
            int fieldToUpdate = req[1] - '0';
            std::string newValue =  webview::detail::json_parse(req, "", 1);

            switch(fieldToUpdate){
                case 0:
                    newLoginName = newValue;
                    break;
                case 1:
                    newLoginUser = newValue;
                    break;
                case 2:
                    newLoginPass = newValue;
                    break;
            }
        },
        nullptr
    );
    w.bind(
        "openLogin",
        [&](const std::string &seq, const std::string &req, void *){
            std::cout << storage.JsonOut() << std::endl;
        },
        nullptr
    );

    w.set_html(html);

    w.run();

    return 1;
}
