#pragma once

#include "Canvas.h"
#include "AppPrefs.h"

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

    static void TryOpen(const std::filesystem::path& filePath);

    static void Exec(const xe::Command& cmd);
    static void Exec(const std::function<void(void)>& execute, const std::function<void(void)>& revert);

    static AppPrefs& Prefs();

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
    std::unique_ptr<sf::RenderWindow> m_window;
    std::unique_ptr<sf::RenderTexture> m_viewport;
    std::unique_ptr<Canvas> m_canvas;

    xe::CommandStack m_cmdStack;

    AppPrefs m_prefs;
    sf::ContextSettings m_windowCtx;
    sf::Clock m_deltaClock;
    sf::Vector2i m_lastMousePos;
    sf::Vector2f m_lastViewportPosition;
    int m_zoomPercent = 100;
    int m_scrollDelta = 0;
    bool m_isSaved = true;
    bool m_showExport = false;
    bool m_showHelp = false;
};
