#include "App.h"

#include "Message.h"
#include "Algorithms.h"
#include "Mathematics.h"

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

void App::Exec(const xe::Command& cmd)
{
    s_inst->m_cmdStack.PushAndExecute(cmd);
}

void App::Exec(const std::function<void(void)>& execute, const std::function<void(void)>& revert)
{
    s_inst->m_cmdStack.PushAndExecute(execute, revert);
}

void App::_Start()
{
    windowCtx.antialiasingLevel = 8;

    sf::Vector2u windowDim = { 1920u, 1080u };

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowDim.x, windowDim.y), "LineDesigner", sf::Style::Default);
    //window->setFramerateLimit(60);

#ifdef WIN32
    HWND hWnd = window->getSystemHandle();
    BOOL useDark = TRUE;
    DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDark, sizeof(useDark));
#endif // WIN32

    viewport = std::make_unique<sf::RenderTexture>();
    viewport->create(1920, 1080, windowCtx);
    {
        sf::View view = viewport->getView();
        view.setCenter({ 0.f, 0.f });
        viewport->setView(view);
    }

    ImGui::SFML::Init(*window);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    canvas = std::make_unique<Canvas>();
    canvas->Initialize();
    canvas->Load("testcanvas.yaml");
}

void App::_Update()
{
    while (window->isOpen())
    {
        scrollDelta = 0;
        sf::Vector2i mousePos = sf::Mouse::getPosition();
        sf::Vector2i mouseDelta = lastMousePos - mousePos;
        lastMousePos = mousePos;

        sf::Event event;
        while (window->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(*window, event);

            if (event.type == sf::Event::Closed)
            {
                window->close();
            }

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                scrollDelta = event.mouseWheelScroll.delta;
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

        ImGui::SFML::Update(*window, deltaClock.restart());
        ImGui::DockSpaceOverViewport();

        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoCollapse);
        ImVec2 availSize = ImGui::GetContentRegionAvail();
        if (availSize.x > 0 && availSize.y > 0)
        {
            sf::Vector2u currSize = viewport->getSize();
            if (currSize.x != (uint32_t)availSize.x || currSize.y != (uint32_t)availSize.y)
            {
                sf::View view = viewport->getView();
                sf::Vector2f center = view.getCenter();

                viewport->create((uint32_t)availSize.x, (uint32_t)availSize.y, windowCtx);
                currSize = viewport->getSize();

                view = viewport->getView();
                view.setCenter(center);
                sf::Vector2f viewSize = (sf::Vector2f)currSize;
                viewSize *= (100.f / zoomPercent);
                view.setSize(viewSize);
                viewport->setView(view);
            }
        }

        if (ImGui::IsWindowHovered())
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
            {
                sf::View view = viewport->getView();
                view.move((sf::Vector2f)mouseDelta * (100.f / zoomPercent));
                viewport->setView(view);
            }
            if (scrollDelta != 0)
            {
                zoomPercent = Algorithm::Clamp(zoomPercent + scrollDelta * 10, 25, 400);
                sf::View view = viewport->getView();
                sf::Vector2f viewSize = (sf::Vector2f)viewport->getSize();
                viewSize *= (100.f / zoomPercent);
                view.setSize(viewSize);

                viewport->setView(view);
            }
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

                xe::Vector2 windowPos = xe::Vector2(ImGui::GetMousePos()) - lastViewportPosition;
                sf::Vector2f worldPos = viewport->mapPixelToCoords(windowPos);
                canvas->TrySelect(worldPos, mod);
            }
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                xe::Vector2 windowPos = xe::Vector2(ImGui::GetMousePos()) - lastViewportPosition;
                sf::Vector2f worldPos = viewport->mapPixelToCoords(windowPos);
                canvas->NewPointCommand(worldPos);
            }
        }
        if (ImGui::IsWindowFocused())
        {
            if (ImGui::IsKeyPressed(ImGuiKey_Delete))
            {
                canvas->TryDelete();
            }
            if (ImGui::IsKeyPressed(ImGuiKey_E))
            {
                canvas->TempExport();
            }
        }

        viewport->clear({ 10, 10, 10 });
        canvas->DrawTo(*viewport);
        viewport->display();

        ImTextureID texID = (void*)viewport->getTexture().getNativeHandle();
        ImGui::Image(texID, availSize, ImVec2(0, 1), ImVec2(1, 0));
        lastViewportPosition = ImGui::GetItemRectMin();

        ImGui::End();

        ImGui::Begin("Inspector");
        ImGui::Text("Zoom: %i", zoomPercent);
        canvas->OnGUI();
        ImGui::End();

        canvas->Update();

        window->clear({ 10, 10, 10 });
        ImGui::SFML::Render(*window);
        window->display();
    }
}

void App::_Shutdown()
{
    ImGui::SFML::Shutdown();
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
