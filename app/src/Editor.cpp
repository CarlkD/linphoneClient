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

#include "Editor.h"

void Editor::AddEditorElements ()
{
    DrawMainWindow();
    if (callWindowOpen) DrawCallWindow();
    if (incomingCallWindowOpen) DrawIncCallWindow();
    // DrawMenuBar();
    // ImGui::ShowDemoWindow();
}

void Editor::DrawMenuBar() 
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows")) 
        {
            // if (ImGui::MenuItem("Audio Settings", NULL, &audioSettingsWindowOpen)) {}
            // if (ImGui::MenuItem("Viewport", NULL, &viewportWindowOpen)) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Editor::DrawMainWindow() {

    ImGui::SetNextWindowPos(ImVec2{0,0});
    ImGui::SetNextWindowSize(ImVec2{400,600});
    ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    //========================== ACCOUNT ==========================

    ImGui::Text("Proxy configuration address");
    static char ownAddress[128] = "";
    ImGui::InputTextWithHint("User address", "Enter SIP address", ownAddress, IM_ARRAYSIZE(ownAddress));
    if(ImGui::Button("Set address"))
        appController->setAccountSetting(ownAddress);

    static string accStatus;
    switch (appController->getModelAccStat()) {
        break;
        case 0:
            accStatus = "Proxy configuration added succesfully and made default.";
        break;
        case 1:
            accStatus = "Proxy configuration could not be added.";
        break;
        default: case -1:
            accStatus = "Undefined.";
    }
    ImGui::Text(accStatus.c_str());

    ImGui::Separator(); 

    //========================== AUTHENTICATION ==========================

    ImGui::Text("Username");
    static char username[128] = "";
    ImGui::InputTextWithHint("Username", "Enter username", username, IM_ARRAYSIZE(username));

    ImGui::Text("Domain");
    static char domain[128] = "";
    ImGui::InputTextWithHint("Domain", "Enter domain address", domain, IM_ARRAYSIZE(domain));

    ImGui::Text("Password");
    static char password[128] = "";
    ImGui::InputTextWithHint("Password", "Enter password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

    if(ImGui::Button("Set authentication"))
    {
        AuthSettings as{username, password, domain};
        appController->setAuthSettings(as);
    }

    static string authStatus;
    switch (appController->getModelAuthStat()) {
        break;
        case 0:
            authStatus = "Successfuly authenticated.";
        break;
        case 1:
            authStatus = "Authentication is not successful.";
        break;
        default: case -1:
            authStatus = "Undefined.";
    }
    ImGui::Text(authStatus.c_str());

    ImGui::Separator(); 

    //========================== CALL ==========================

    ImGui::Text("Destination SIP adress");
    static char destAddress[128] = "";
    ImGui::InputTextWithHint("Calling address", "Enter SIP address to call", destAddress, IM_ARRAYSIZE(destAddress));
    if(ImGui::Button("Call"))
    {
        appController->makeCallToAddress(destAddress);
        callWindowOpen = true;
    }

    ImGui::Separator(); 

    //========================== TEST ==========================

    if(ImGui::Button("Incoming Call Window Test"))
        incomingCallWindowOpen = true;

    ImGui::Separator(); 

    ImGui::End();
}

void Editor::DrawCallWindow() {

    ImGui::SetNextWindowPos(ImVec2{0,300});
    ImGui::SetNextWindowSize(ImVec2{400,300});

    ImGui::Begin("Call Window", &callWindowOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    std::string callerName = "Calling: " + appController->getCallerName();
    ImGui::Text(callerName.c_str());

    if (ImGui::Button("End Call"))
    {
        appController->endCall();
        callWindowOpen = false;
    }


    if (ImGui::Button("Start Video"))
    {
        appController->setVideoStatus(true);
        videoEnabled = true;
    }

    if (videoEnabled)
    {
        if (ImGui::Button("Stop Video"))
        {
            appController->setVideoStatus(false);
            videoEnabled = false;
        }
    }

    ImGui::End();
}

void Editor::DrawIncCallWindow()
{
    ImGui::SetNextWindowPos(ImVec2{0,300});
    ImGui::SetNextWindowSize(ImVec2{400,300});

    ImGui::Begin("Incoming Call Window", &incomingCallWindowOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // std::string callerName = "Incoming call: " + appController->getCallerName();
    ImGui::Text("callerName.c_str()");

    if (ImGui::Button("Accept"))
    {
        incomingCallWindowOpen = false;
        // callWindowOpen = true;
    }


    if (ImGui::Button("Deny"))
    {
        incomingCallWindowOpen = false;
    }

    ImGui::End();
}


void Editor::DrawGraphics()
{
        if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        throw("Failed to initialize GLFW.");
        return;
    }
    else
    {
        std::cout << "[INFO] GLFW initialized\n";
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER , 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_REFRESH_RATE, 30);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
        );

    std::string glsl_version = "";

#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint( // required on Mac OS
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE
        );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    GLFWwindow *window = glfwCreateWindow(
        400,
        600,
        "linphoneApp",
        NULL,
        NULL
        );
    if (!window)
    {
        std::cerr << "[ERROR] Couldn't create a GLFW window\n";
        teardown();
        return;
    }
    // watch window resizing
    glfwMakeContextCurrent(window);
    // VSync
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
        teardown();
        return;
    }
    else
    {
        std::cout << "[INFO] GLAD initialized\n";
    }

    std::cout << "[INFO] OpenGL from glad "
              << GLVersion.major << "." << GLVersion.minor
              << std::endl;

    int actualWindowWidth, actualWindowHeight;
    glfwGetWindowSize(window, &actualWindowWidth, &actualWindowHeight);
    glViewport(0, 0, actualWindowWidth, actualWindowHeight);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();   // setup Dear ImGui style

    ImGui_ImplGlfw_InitForOpenGL(window, true); // setup platform/renderer bindings
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    // --- rendering loop
    ChangeStyle();
    // io.Fonts->AddFontFromFileTTF("verdana.ttf", 13.0f);
    while (!glfwWindowShouldClose(window))
    {
        glViewport(1024, 768, 0,0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render your GUI
        AddEditorElements();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glDrawArrays(GL_POINTS, 0, 12);

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
    }

    
}

void Editor::teardown()
{
    if (window != NULL) 
        glfwDestroyWindow(window);

    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Editor::ChangeStyle()
{
    // Setup style
ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.74f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.70f, 0.72f, 0.56f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.33f, 0.14f, 0.47f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.97f, 0.31f, 0.13f, 0.81f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.65f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);//ImVec4(0.46f, 0.84f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
    style.Alpha = 1.0f;
    style.FrameRounding = 4;
    style.IndentSpacing = 12.0f;
}