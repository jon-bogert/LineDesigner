#pragma once

#include "Canvas.h"

#include <XephTools/CommandStack.h>

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

    static void Exec(const xe::Command& cmd);
    static void Exec(const std::function<void(void)>& execute, const std::function<void(void)>& revert);

private:
    void _Start();
    void _Update();
    void _Shutdown();

    void _New();
    bool _Load(const std::filesystem::path& path);
    bool _Load();
    bool _Save(const std::filesystem::path& path);
    bool _Save(bool forceNew = false);

    bool _CheckSave();

    void _Undo();
    void _Redo();

private:
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::RenderTexture> viewport;
    std::unique_ptr<Canvas> canvas;

    xe::CommandStack m_cmdStack;

    sf::ContextSettings windowCtx;
    sf::Clock deltaClock;
    sf::Vector2i lastMousePos;
    sf::Vector2f lastViewportPosition;
    int zoomPercent = 100;
    int scrollDelta = 0;
    bool m_isSaved = true;
};
