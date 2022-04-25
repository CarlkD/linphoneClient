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

#include <string>

using std::string;

struct AuthSettings
{
    AuthSettings(const string& un, const string& pw, const string& dm): username(un), password(pw), domain(dm) {};
    string username;
    string password;
    string domain;
};

/**
 * @brief Abstract class to use between Editor - Model interaction. 
 * Desirable virtual functions can be implemented in the derived class. 
 * Works like an interface in between. 
 * 
 */
class Controller
{
public:
    virtual void makeCallToAddress(const string& sip) {}
    virtual void endCall() {}
    virtual string getCallerName() {return "";} 

    virtual void setAuthSettings(const AuthSettings& as) {}
    virtual void setAccountSetting(const string& as) {}

    virtual int getModelAuthStat() {}
    virtual int getModelAccStat() {} 

    virtual void setVideoStatus(bool st) {}

    virtual void setMicMuted(bool m) {}
    virtual void setMicGain(float g) {}
};