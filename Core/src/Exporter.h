#pragma once

#include "Mathematics.h"
#include "XephTools/FileBrowser.h"

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <stb_image_write.h>

#include <functional>

struct Exporter
{
public:
	sf::Color backgroundColor = sf::Color::Transparent;
	sf::Vector2i dimensions = {1024, 1024};
	float scale = 1.f;

	sf::RenderTexture target;
	std::function<void(sf::RenderTarget&)> drawCallback;

	void OnGUI()
	{
		sf::ContextSettings ctx;
		ctx.antialiasingLevel = 8;
		dimensions.x = xe::Math::Max(dimensions.x, 0.f);
		dimensions.y = xe::Math::Max(dimensions.y, 0.f);
		scale = xe::Math::Max(scale, 0.001f);

		target.create((uint32_t)dimensions.x, (uint32_t)dimensions.y, ctx);

		sf::View view = target.getView();
		view.setCenter({ 0.f, 0.f });
		view.setSize((sf::Vector2f)dimensions * (1.f / scale));
		target.setView(view);
		target.clear(backgroundColor);
		drawCallback(target);
		target.display();

		float previewScale = 512.f / dimensions.y;

		ImTextureID texID = (void*)target.getTexture().getNativeHandle();
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5f - (0.5f * dimensions.x * previewScale));
		ImGui::Image(texID, (sf::Vector2f)dimensions * previewScale, ImVec2(0, 1), ImVec2(1, 0), sf::Color::White, sf::Color(127, 127, 127));

		ImGui::Separator();

		ImGui::DragInt2("Image Size##Export", &dimensions.x);
		ImGui::DragFloat("Content Scale##Export", &scale, 0.001);
		xe::Color bgColor = xe::Color8(backgroundColor);
		if (ImGui::ColorEdit4("Background Color##Export", &bgColor.r))
		{
			backgroundColor = bgColor.As8bit();
		}
		ImGui::NewLine();
		if (ImGui::Button("Export"))
		{
			xe::FileBrowser browser;
			browser.PushFileType(L"*.png", L"PNG Image");
			std::filesystem::path path = browser.SaveFile();
			if (!path.empty())
			{
				sf::Image img = target.getTexture().copyToImage();
				stbi_write_png(path.string().c_str(), dimensions.x, dimensions.y, 4, img.getPixelsPtr(), dimensions.x * 4);
			}
		}
	}
};