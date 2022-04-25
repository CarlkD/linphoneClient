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
#include <chrono>
#include <thread>

#include <core.hh>
#include <factory.hh>
#include <audio_device.hh>
#include <core_listener.hh>
#include <address.hh>
#include <account.hh>
#include <account_params.hh>
#include <auth_info.hh>
#include <proxy_config.hh>
#include <core_listener.hh>
#include <call_params.hh>
#include <transports.hh>

using std::string;
using std::cout;
using std::endl;

using linphone::Core;
using linphone::Factory;
using linphone::Call;
using linphone::AudioDevice;
using linphone::CoreListener;
using linphone::AuthInfo;
using linphone::CallParams;

enum ModelState{
    Stopped,
    Running
};

enum AuthenticationStatus{
    Authenticated,
    NotAuthenticated = -1
};

enum AccountStatus{
    AccountIsSet,
    NoAccoutSet = -1
};

/**
 * @brief Main model that implements linphone behaviour. 
 * 
 */
class PhoneModel : public CoreListener
{
public:
    /**
     * @brief Construct a new Phone Model object. Initializes a shared pointer to own and a shared pointer of type Core Listener with it's own.
     * 
     */
    PhoneModel(): own{std::shared_ptr<PhoneModel>(this)}, coreListener {std::shared_ptr<PhoneModel>(this)} {}

    /**
     * @brief Destroy the Phone Model object
     * 
     */
    ~PhoneModel(){};

    /**
     * @brief Creates linphone Factory object. Creates linphone Core with the Factory object. 
     * Adds this object as a Core Listener to the Core. Logs Core status. 
     * 
     */
    void initialize();

    /**
     * @brief Iterates linphone Core as long as @ref ModelState::Running.
     * 
     */
    void run();

    /**
     * @brief Can be called to set @ref ModelState::Stopped. This will end the iteration process in the @ref run() loop. 
     * 
     */
    void end();

    /**
     * @brief Makes call to the SIP address passed. Sets the own call pointer as a reference to the created call. 
     * Model needs to be initialized and Core needs to be iterating for the Call to be started. State can be tracked with @ref onCallStateChanged() function.
     * 
     * @param dest 
     */
    void makeCall(const string& dest);

    /**
     * @brief Ends the ongoing call. Terminates the call. 
     * TODO: This action sometimes aborts the whole application: Call states goes to Error after End.
     * 
     */
    void endCall();

    void enableVideo(bool b);

    /**
     * 
     * @brief Adds the authentication info to the core and sets it as the default AuthInfo.
     * 
     * @param username Username
     * @param password Password
     * @param domain Domain
     */
    void setAuthentication(const string& username, const string& password, const string& domain);

    /**
     * @brief Creates a new account and adds it to the Core. Makes it the default account. 
     * 
     * @param sip Address for the Account
     * @return int 0 if successful 1 if not.
     */
    void setAccount(const string& sip);

    /**
     * @brief TODO: Integrate
     * 
     * @param m 
     */
    void muteMic(bool m) {call->setMicrophoneMuted(m); }

    /**
     * @brief returns the address of the current caller from the Call;
     * 
     * @return string Address of the caller.
     */
    string getCallerName() {auto address = call->getRemoteAddress(); return address->asString();}

    /**
     * @brief Overriden from the abstract class Core Listener. 
     * TODO: Inform new call. Metion ringing while outgoing a new call. Display call status in the editor.
     * 
     * @param core 
     * @param call 
     * @param state 
     * @param message 
     */
    void onCallStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call, linphone::Call::State state, const std::string & message);

    AuthenticationStatus getAuthanticationStatus() {return authStat;}
    AccountStatus getAccountStatus() {return accStat;} 
    
private:
    std::shared_ptr<PhoneModel> own;

    std::shared_ptr<Core> linCore;
    std::shared_ptr<Factory> linFactory;
    std::shared_ptr<Call> call;
    std::shared_ptr<AuthInfo> userAuthInfo;
    std::shared_ptr<CoreListener> coreListener;
    std::shared_ptr<CallParams> callParameters;

    ModelState state = ModelState::Stopped;
    string callAdress;

    AuthenticationStatus authStat;
    AccountStatus accStat;
};