#include "Algorithms.h"
#include "Canvas.h"
#include "Mathematics.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

#ifdef WIN32
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif // WIN32

#include <XephTools/EntryPoint.h>

int Main(const std::vector<std::string>& args)
{
    sf::ContextSettings windowCtx;
    windowCtx.antialiasingLevel = 8;

    sf::Vector2u windowDim = { 1920u, 1080u };

    sf::RenderWindow window(sf::VideoMode(windowDim.x, windowDim.y), "LineDesigner", sf::Style::Default);
    //window.setFramerateLimit(60);

#ifdef WIN32
    HWND hWnd = window.getSystemHandle();
    BOOL useDark = TRUE;
    DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDark, sizeof(useDark));
#endif // WIN32

    sf::RenderTexture viewport;
    viewport.create(1920, 1080, windowCtx);
    {
        sf::View view = viewport.getView();
        view.setCenter({ 0.f, 0.f });
        viewport.setView(view);
    }

    ImGui::SFML::Init(window);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    Canvas canvas;
    canvas.Load("testcanvas.yaml");

    sf::Clock deltaClock;
    sf::Vector2i lastMousePos;
    sf::Vector2f lastViewportPosition;
    int zoomPercent = 100;
    int scrollDelta = 0;

    while (window.isOpen())
	{
        scrollDelta = 0;
        sf::Vector2i mousePos = sf::Mouse::getPosition();
        sf::Vector2i mouseDelta = lastMousePos - mousePos;
        lastMousePos = mousePos;

        sf::Event event;
        while (window.pollEvent(event))
		{
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
			{
                window.close();
            }

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                scrollDelta = event.mouseWheelScroll.delta;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::DockSpaceOverViewport();

        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoCollapse);
        ImVec2 availSize = ImGui::GetContentRegionAvail();
        if (availSize.x > 0 && availSize.y > 0)
        {
            sf::Vector2u currSize = viewport.getSize();
            if (currSize.x != (uint32_t)availSize.x || currSize.y != (uint32_t)availSize.y)
            {
                sf::View view = viewport.getView();
                sf::Vector2f center = view.getCenter();

                viewport.create((uint32_t)availSize.x, (uint32_t)availSize.y, windowCtx);
                currSize = viewport.getSize();

                view = viewport.getView();
                view.setCenter(center);
                sf::Vector2f viewSize = (sf::Vector2f)currSize;
                viewSize *= (100.f / zoomPercent);
                view.setSize(viewSize);
                viewport.setView(view);
            }
        }

		if (ImGui::IsWindowHovered())
		{
			if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
			{
				sf::View view = viewport.getView();
				view.move((sf::Vector2f)mouseDelta * (100.f / zoomPercent));
				viewport.setView(view);
			}
            if (scrollDelta != 0)
            {
                zoomPercent = Algorithm::Clamp(zoomPercent + scrollDelta * 10, 25, 400);
                sf::View view = viewport.getView();
                sf::Vector2f viewSize = (sf::Vector2f)viewport.getSize();
                viewSize *= (100.f / zoomPercent);
                view.setSize(viewSize);

                viewport.setView(view);
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
                sf::Vector2f worldPos = viewport.mapPixelToCoords(windowPos);
                canvas.TrySelect(worldPos, mod);
            }
        }

        viewport.clear({ 10, 10, 10 });
        canvas.DrawTo(viewport);
        viewport.display();

        ImTextureID texID = (void*)viewport.getTexture().getNativeHandle();
        ImGui::Image(texID, availSize, ImVec2(0, 1), ImVec2(1, 0));
        lastViewportPosition = ImGui::GetItemRectMin();

        ImGui::End();

        ImGui::Begin("Inspector");
        ImGui::Text("Zoom: %i", zoomPercent);
        canvas.OnGUI();
        ImGui::End();

        canvas.Update();

        window.clear({10, 10, 10});
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

SetEntryPointA(Main)