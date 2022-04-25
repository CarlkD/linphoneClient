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

#include <Model.h>

void PhoneModel::initialize()
{
    linFactory = Factory::get();
    linCore = linFactory->createCore("config.ini", "factoryconfig.ini", nullptr);

    auto transports = linFactory->createTransports();
    transports->setUdpPort(5060);
    transports->setTcpPort(0);
    transports->setTlsPort(0);
    linCore->setTransports(transports);

    linCore->addListener(std::shared_ptr<CoreListener>(this));

    linCore->enableIpv6(true);
    linCore->enableAdaptiveRateControl(true);
    linCore->setIncTimeout(60);

    if(linCore->getGlobalState() == linphone::GlobalState::Ready)
    {
        linCore->start();
        std::cout << "Core has started." << std::endl;
    }

    if(linCore->getGlobalState() == linphone::GlobalState::On)
        std::cout << "Core is up and running." << std::endl;

    return;
}


void PhoneModel::run()
{
    state = ModelState::Running;
    do
    {
        linCore->iterate();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    while(state == ModelState::Running);

    linCore->stop();
}

void PhoneModel::end()
{
    state = ModelState::Stopped;
    return;
}

void PhoneModel::makeCall(const string& dest)
{
    call = linCore->invite(dest);
    callParameters = linCore->createCallParams(call);

    if(call == nullptr)
    {
        cout << "Can't make call." << endl;
    }
    else
        cout << "Call is in progress." << endl;

    call->setNativeVideoWindowId(0);

    auto audioDeviceList = linCore->getAudioDevices();

    for(auto ad: audioDeviceList)
    {
        if (ad->getDeviceName() == "Mac mini Speakers (BuiltInSpeakerDevice)")
            call->setOutputAudioDevice(ad);
    }

    auto inputDevice = call->getInputAudioDevice();
    auto outputDevice = call->getOutputAudioDevice();
}

void PhoneModel::endCall()
{   
    cout << "Call ended." << endl;
    if(call->getState() == linphone::Call::State::OutgoingRinging)
        linCore->stopRinging();

    call->pause();
    call->terminate();

        return;
}

void PhoneModel::enableVideo(bool b)
{

    callParameters->enableVideo(b);
    call->update(callParameters);
}

void PhoneModel::setAuthentication(const string& username, const string& password, const string& domain)
{
    userAuthInfo = linFactory->createAuthInfo(username, "", password, "", "", domain);
    
    linCore->addAuthInfo(userAuthInfo);

    if (linCore->findAuthInfo(userAuthInfo->getRealm(), userAuthInfo->getUsername(), userAuthInfo->getDomain()) != nullptr)
    {
        std::cout << "Authentication successful" << std::endl;
        authStat = Authenticated;
    }
    else {
        std::cout << "Authentication was not successful" << std::endl;
        authStat = NotAuthenticated;
    }
        
    return;
}

void PhoneModel::setAccount(const string& sip)
{
    auto address = linFactory->createAddress(sip); 
    auto accountParams = linCore->createAccountParams();
    accountParams->setIdentityAddress(address);
    accountParams->setServerAddress(address);
    accountParams->setExpires(360000);
    accountParams->setPublishExpires(360000);
    auto account = linCore->createAccount(accountParams);
    linCore->addAccount(account);
    linCore->setDefaultAccount(account);

    if (linCore->getDefaultAccount() == account)
    {
        std::cout << "Default account is set." << std::endl;
        accStat = AccountIsSet;
    }
    else {
        std::cout << "Default account couldn't be set." << std::endl;
        accStat = NoAccoutSet;
    }
        
    return;
}

void PhoneModel::onCallStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call, linphone::Call::State state, const std::string & message)
{
    std::cout << (int) state << std::endl;

    // switch (state) {
    //    
        
    // }
}
// int MakeCall()
// {
//     string dest = "sip:+436704080324@sip.linphone.org";

//     auto linFac = Factory::get();
//     auto linCore = linFac->createCore("../config.ini", "../factoryconfig.ini", nullptr);

//     if(linCore->getGlobalState() == linphone::GlobalState::Ready)
//     {
//         linCore->start();
//         std::cout << "Core has started." << std::endl;
//     }

//     if(linCore->getGlobalState() == linphone::GlobalState::On)
//         std::cout << "Core is up and running." << std::endl;

//     auto call = linCore->invite(dest);
//     if(call == nullptr)
//     {
//         cout << "Can't make call." << endl;
//     }
//     else
//         cout << "Call is in progress." << endl;

//     call->setMicrophoneMuted(false);
//     auto audioDeviceList = linCore->getAudioDevices();

//     for(auto ad: audioDeviceList)
//     {
//         if (ad->getDeviceName() == "Mac mini Speakers (BuiltInSpeakerDevice)")
//             call->setOutputAudioDevice(ad);
//     }

//     auto inputDevice = call->getInputAudioDevice();
//     auto outputDevice = call->getOutputAudioDevice();

//     cout << "Input device: " << inputDevice->getDeviceName() << endl;
//     cout << "Output device: " << outputDevice->getDeviceName() << endl;

//     do
//     {
//         linCore->iterate();
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
//     while(true);


//     linCore->stop();
//     return 0;
// }