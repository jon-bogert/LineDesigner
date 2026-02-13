#include "Algorithms.h"
#include "Canvas.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <XephTools/EntryPoint.h>

int Main(const std::vector<std::string>& args)
{
    sf::ContextSettings windowCtx;
    windowCtx.antialiasingLevel = 8;

    sf::Vector2u windowDim = { 1920u, 1080u };

    sf::RenderWindow window(sf::VideoMode(windowDim.x, windowDim.y), "LineDesigner", sf::Style::Default, windowCtx);
    //window.setFramerateLimit(60);
    {
        sf::View view = window.getView();
        view.setCenter({0.f, 0.f});
        window.setView(view);
    }

    ImGui::SFML::Init(window);

    Canvas canvas;
    canvas.Load("testcanvas.yaml");

    sf::Clock deltaClock;
    sf::Vector2i lastMousePos;
    int zoomPercent = 100;

    while (window.isOpen())
	{
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

            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
                if (event.type == sf::Event::MouseMoved)
                {
                    sf::View view = window.getView();
                    view.move((sf::Vector2f)mouseDelta * (100.f / zoomPercent));
                    window.setView(view);
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                int scrollDelta = event.mouseWheelScroll.delta;
                zoomPercent = Algorithm::Clamp(zoomPercent + scrollDelta * 10, 25, 400);
                sf::View view = window.getView();
                sf::Vector2f winSize = (sf::Vector2f)window.getSize();
                winSize *= (100.f / zoomPercent);
                view.setSize(winSize);

                window.setView(view);
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Button::Left)
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
                    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    canvas.TrySelect(worldPos, mod);
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Text("Zoom: %i", zoomPercent);
        canvas.OnGUI();
        ImGui::End();

        window.clear({10, 10, 10});
        canvas.DrawTo(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

SetEntryPointA(Main)