#pragma once

#include "Canvas.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <memory>

class App final
{
    App() {}
public:

    ~App() = default;
    App(const App& other) = delete;
    App(const App&& other) = delete;
    App operator=(const App& other) = delete;
    App operator=(const App&& other) = delete;

    static void Initialize();
    static void Update();
    static void Shutdown();

private:
    void _Start();
    void _Update();
    void _Shutdown();

private:
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::RenderTexture> viewport;
    std::unique_ptr<Canvas> canvas;

    sf::ContextSettings windowCtx;
    sf::Clock deltaClock;
    sf::Vector2i lastMousePos;
    sf::Vector2f lastViewportPosition;
    int zoomPercent = 100;
    int scrollDelta = 0;
};
