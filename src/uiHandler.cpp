#include "uiHandler.hpp"

#include "webview.h"

#include <string>
#include <iostream>

constexpr const auto html =
R"html(
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
    var textDiv = document.getElementById('right-section');
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

    //
  </script>
)html";

void initializeUI(webview::webview *w){
    unsigned int count = 0;

    w->set_title("Bind Example");
    w->set_size(480, 320, WEBVIEW_HINT_NONE);

    //binds
        // seq -> sequential id of the request
        // req -> json array with all arguments passed
        // resolve returns the function (resolved promise), resolve(request id, status, return value)
    
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

    w->set_html(html);
    w->run();
}