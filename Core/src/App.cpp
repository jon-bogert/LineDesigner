#include "App.h"

#include "Message.h"
#include "Algorithms.h"
#include "Mathematics.h"
#include "Style.h"

#include <XephTools/FileBrowser.h>

#ifdef WIN32
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif // WIN32

static App* s_inst = nullptr;

void App::Initialize()
{
	if (s_inst != nullptr)
	{
		Message::ErrorNotice("Application instance already initialized");
		return;
	}

	s_inst = new App();
	s_inst->_Start();
}

void App::Update()
{
	s_inst->_Update();
}

void App::Shutdown()
{
	if (s_inst == nullptr)
	{
		Message::ErrorNotice("No Application instance to shutdown");
		return;
	}

	s_inst->_Shutdown();

	delete s_inst;
	s_inst = nullptr;
}

void App::TryOpen(const std::filesystem::path& filePath)
{
    s_inst->_Load(filePath);
}

void App::Exec(const xe::Command& cmd)
{
    s_inst->m_isSaved = false;
    s_inst->m_cmdStack.PushAndExecute(cmd);
}

void App::Exec(const std::function<void(void)>& execute, const std::function<void(void)>& revert)
{
    s_inst->m_isSaved = false;
    s_inst->m_cmdStack.PushAndExecute(execute, revert);
}

AppPrefs& App::Prefs()
{
    return s_inst->m_prefs;
}

void App::_Start()
{
    m_prefs.Load();
    m_windowCtx.antialiasingLevel = 8;

    sf::Vector2u windowDim = { m_prefs.windowWidth, m_prefs.windowHeight };

    m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowDim.x, windowDim.y), "Line Designer", sf::Style::Default);
    //window->setFramerateLimit(60);

#ifdef WIN32
    HWND hWnd = m_window->getSystemHandle();
    BOOL useDark = TRUE;
    DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDark, sizeof(useDark));
#endif // WIN32

    m_viewport = std::make_unique<sf::RenderTexture>();
    m_viewport->create(windowDim.x, windowDim.y, m_windowCtx);
    {
        sf::View view = m_viewport->getView();
        view.setCenter({ 0.f, 0.f });
        m_viewport->setView(view);
    }

    ImGui::SFML::Init(*m_window);
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->Clear();
    std::filesystem::path fontpath = "C:\\Windows\\Fonts\\" + m_prefs.fontName;
    io.Fonts->AddFontFromFileTTF(fontpath.string().c_str(), m_prefs.fontSize);
    ImGui::SFML::UpdateFontTexture();
    
    
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    SetUIStyle();
    m_canvas = std::make_unique<Canvas>();
    m_canvas->Initialize();
}

void App::_Update()
{
    while (m_window->isOpen())
    {
        m_scrollDelta = 0;
        sf::Vector2i mousePos = sf::Mouse::getPosition();
        sf::Vector2i mouseDelta = m_lastMousePos - mousePos;
        m_lastMousePos = mousePos;

        sf::Event event;
        while (m_window->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(*m_window, event);

            if (event.type == sf::Event::Closed)
            {
                if (_CheckSave())
                {
                    m_window->close();
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                m_scrollDelta = event.mouseWheelScroll.delta;
            }

            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            {
                if (event.key.code == sf::Keyboard::Key::Z)
                {
                    _Undo();
                }
                else if (event.key.code == sf::Keyboard::Key::Y)
                {
                    _Redo();
                }
            }
        }

        ImGui::SFML::Update(*m_window, m_deltaClock.restart());
        ImGui::DockSpaceOverViewport();
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                _New();
            }

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                _Load();
            }

            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                _Save();
            }

            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
            {
                _Save(true);
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Export", "Ctrl+E"))
            {
                m_showExport = true;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
            {
                if (_CheckSave())
                {
                    m_window->close();
                }
            }

            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();

        int flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoCollapse;
        if (!m_isSaved)
            flags |= ImGuiWindowFlags_UnsavedDocument;

        ImGui::Begin("Viewport", nullptr, flags);

        ImVec2 availSize = ImGui::GetContentRegionAvail();
        if (availSize.x > 0 && availSize.y > 0)
        {
            sf::Vector2u currSize = m_viewport->getSize();
            if (currSize.x != (uint32_t)availSize.x || currSize.y != (uint32_t)availSize.y)
            {
                sf::View view = m_viewport->getView();
                sf::Vector2f center = view.getCenter();

                m_viewport->create((uint32_t)availSize.x, (uint32_t)availSize.y, m_windowCtx);
                currSize = m_viewport->getSize();

                view = m_viewport->getView();
                view.setCenter(center);
                sf::Vector2f viewSize = (sf::Vector2f)currSize;
                viewSize *= (100.f / m_zoomPercent);
                view.setSize(viewSize);
                m_viewport->setView(view);
            }
        }

        if (ImGui::IsWindowHovered())
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
            {
                sf::View view = m_viewport->getView();
                view.move((sf::Vector2f)mouseDelta * (100.f / m_zoomPercent));
                m_viewport->setView(view);
            }
            if (m_scrollDelta != 0)
            {
                m_zoomPercent = Algorithm::Clamp(m_zoomPercent + m_scrollDelta * 10, 25, 400);
                sf::View view = m_viewport->getView();
                sf::Vector2f viewSize = (sf::Vector2f)m_viewport->getSize();
                viewSize *= (100.f / m_zoomPercent);
                view.setSize(viewSize);

                m_viewport->setView(view);
            }

            Gizmo& gizmo = m_canvas->GetGizmo();
            if (m_canvas->IsGizmoActive())
            {
                if (gizmo.IsDragging())
                {
                    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    {
                        sf::View view = m_viewport->getView();
                        sf::Vector2f moveAmount = gizmo.Move((sf::Vector2f)mouseDelta * (100.f / m_zoomPercent));
                        m_canvas->MoveSelection(moveAmount);
                    }
                    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                    {
                        m_canvas->EndMoveSelection();
                        gizmo.EndDragging();
                    }
                }
                else
                {
                    sf::View view = m_viewport->getView();
                    sf::View defaultView = m_viewport->getDefaultView();
                    float scale = view.getSize().y / defaultView.getSize().y;
                    xe::Vector2 windowPos = xe::Vector2(ImGui::GetMousePos()) - m_lastViewportPosition;
                    sf::Vector2f worldPos = m_viewport->mapPixelToCoords(windowPos);
                    gizmo.CheckHover(scale, worldPos);

                    if (gizmo.IsHovering() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        gizmo.StartDragging();
                        m_canvas->StartMoveSelection();
                    }
                }
            }
            if (!m_canvas->IsGizmoActive() || !gizmo.IsHovering())
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    Canvas::ClickModifier mod = Canvas::ClickModifier::Primary;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    {
                        mod = Canvas::ClickModifier::Add;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                    {
                        mod = Canvas::ClickModifier::Secondary;
                    }

                    xe::Vector2 windowPos = xe::Vector2(ImGui::GetMousePos()) - m_lastViewportPosition;
                    sf::Vector2f worldPos = m_viewport->mapPixelToCoords(windowPos);
                    m_canvas->TrySelect(worldPos, mod);
                }
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    xe::Vector2 windowPos = xe::Vector2(ImGui::GetMousePos()) - m_lastViewportPosition;
                    sf::Vector2f worldPos = m_viewport->mapPixelToCoords(windowPos);
                    m_canvas->NewPointCommand(worldPos);
                }
            }
            
        }
        if (ImGui::IsWindowFocused())
        {
            if (ImGui::IsKeyPressed(ImGuiKey_Delete))
            {
                m_canvas->TryDelete();
            }
        }

        m_viewport->clear({ 10, 10, 10 });
        m_canvas->DrawTo(*m_viewport);
        m_viewport->display();

        ImTextureID texID = (void*)m_viewport->getTexture().getNativeHandle();
        ImGui::Image(texID, availSize, ImVec2(0, 1), ImVec2(1, 0));
        m_lastViewportPosition = ImGui::GetItemRectMin();

        ImGui::End();

        ImGui::Begin("Inspector");
        ImGui::Text("Zoom: %i", m_zoomPercent);
        m_canvas->OnInspectorGUI();
        ImGui::End();

        m_canvas->Update();

        if (m_showExport)
        {
            ImGui::Begin("Export", &m_showExport, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
            m_canvas->OnExportGUI();
            ImGui::End();
        }

        m_window->clear({ 10, 10, 10 });
        ImGui::SFML::Render(*m_window);
        m_window->display();
    }
}

void App::_Shutdown()
{
    m_prefs.windowWidth = m_window->getSize().x;
    m_prefs.windowHeight = m_window->getSize().y;
    m_prefs.Save();

    ImGui::SFML::Shutdown();
}

void App::_New()
{
    if (!_CheckSave())
        return;

    m_cmdStack.Clear();
    m_isSaved = true;
    m_canvas = std::make_unique<Canvas>();
    m_canvas->Initialize();
}

bool App::_Load(const std::filesystem::path& path)
{
    if (!_CheckSave())
        return true;

    _New();
    if (!m_canvas->Load(path))
    {
        _New();
        std::stringstream os;
        os << "Could not read file: " << path;
        Message::ErrorNotice(os);
        return false;
    }
    m_canvas->Initialize();
    return true;
}

bool App::_Load()
{
    if (!_CheckSave())
        return true;

    xe::FileBrowser browser;
    browser.PushFileType(L"*.lines;*.yaml;*.yml", L"Line Designer File");
    std::filesystem::path path = browser.GetFile();
    if (path.empty())
        return false;

    return _Load(path);
}

bool App::_Save(const std::filesystem::path& path)
{
    if (!m_canvas->Save(path))
    {
        std::stringstream os;
        os << "There was a problem saving to file: " << path;
        return false;
    }
    m_isSaved = true;
    return true;
}

bool App::_Save(bool forceNew)
{
    if (m_canvas->GetPath().empty() || forceNew)
    {
        AppPrefs& prefs = App::Prefs();

        xe::FileBrowser browser;
        if (!prefs.lastSave.empty())
        {
            browser.SetStartPath(prefs.lastSave);
        }

        browser.PushFileType(L"*.lines;*.yaml;*.yml", L"Line Designer File");
        std::filesystem::path path = browser.SaveFile();
        if (path.empty())
            return false;

        if (!path.has_extension()
            || (path.extension() != L".lines" && path.extension() != L".yaml" && path.extension() != L".yml"))
        {
            path += L".lines";
        }

        prefs.lastSave = path.parent_path();
        m_canvas->SetPath(path);
    }

    return _Save(m_canvas->GetPath());
}

bool App::_CheckSave()
{
    if (m_isSaved)
        return true;

    Message::Result reuslt = Message::SaveBox();
    switch (reuslt)
    {
    case Message::Yes:
        return _Save();
    case Message::No:
        return true;
    case Message::Cancel:
        return false;
    }

    return false;
}

void App::_Undo()
{
    try
    {
        m_cmdStack.Undo();
    }
    catch (std::exception e)
    {
        Message::ErrorNotice(std::string("Error while trying to undo: ") + e.what());
    }
}

void App::_Redo()
{
    m_cmdStack.Redo();
}
