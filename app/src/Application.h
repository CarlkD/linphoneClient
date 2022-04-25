// MIT License

// Copyright (c) 2022 Doğuş Karlık

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <iostream>
#include <string>
#include <thread>


#include <Editor.h>
#include <Model.h>

using std::string;

/**
 * @brief Application class holds Model (linPhone) and @ref Editor. Responsible for initialization and concurrent actions of each. 
 * Manages the whole application lifetime. Initialization of @ref Editor and model are done during the construction of the object.
 * 
 * 
 */
class Application : public Controller
{
public:
    Application();
    ~Application() {};

    void run();
    void teardown();

    void makeCallToAddress(const string& sip) override {linPhone.makeCall(sip); }
    void endCall() override {linPhone.endCall(); }

    void setAuthSettings(const AuthSettings& as) override {linPhone.setAuthentication(as.username, as.password, as.domain); }
    void setAccountSetting(const string& sip) override {linPhone.setAccount(sip); }

    int getModelAuthStat() override {return (int) linPhone.getAuthanticationStatus(); }
    int getModelAccStat() override {return (int) linPhone.getAccountStatus(); }

    virtual void setVideoStatus(bool st) override {linPhone.enableVideo(st); };

    string getCallerName() override {return linPhone.getCallerName();}
private:
    PhoneModel linPhone;
    Editor appEditor;

    void runPhoneModel();
    void drawGraphics();
};


