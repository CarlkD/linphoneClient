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

#include <glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Controller.h>

    

/**
 * @brief Editor class for application GUI. Responsible for window creation, GUI layout and graphics rendering.
 * 
 */
class Editor {
public:
    /**
     * @brief Construct a new Editor object
     * 
     */
    Editor() {};

    /**
     * @brief Construct a new Editor object and initialize a Controller for application control.
     * 
     * @param controller Controller instance passed to be used as an interface between the model.
     */
    Editor(Controller* controller): appController{controller} {};

    /**
     * @brief Destroy the Editor object
     * 
     */
    ~Editor() {};

    /**
     * @brief Makes a window request to the system using GLFW. Window is returned as the 
     * GLFWWindow* object. Initializes graphics rendering pipeline. Loops until the window should close. 
     * Private draw functions are called each loop/frame.
     * 
     */
    void DrawGraphics();


private:  
    GLFWwindow* window;
    Controller* appController;

    void teardown();
    /**
     * @brief Switches on/off editor widgets that needs to be drawn to the window.
     * 
     */
    void AddEditorElements ();
    /**
     * @brief Draws a main menu bar to the main window. (Deprecated)
     * 
     */
    void DrawMenuBar();
    /**
     * @brief Draws call window GUI starting from the half of the screen.
     * 
     */
    void DrawCallWindow();
    /**
     * @brief Draws main GUI elements.
     * 
     */
    void DrawMainWindow();

    void DrawIncCallWindow();

    bool callWindowOpen = false;
    bool videoEnabled;
    bool incomingCallWindowOpen = false;

    void ChangeStyle();
};
