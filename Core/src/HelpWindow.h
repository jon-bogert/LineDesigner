#pragma once

#include <imgui.h>

void HelpWindowGUI()
{
    ImGui::Text(R"( - You can add points by DOUBLE-CLICKING the left mouse button in the viewport.
 - You can select a point by clicking on it.
 - A point is selected when you see the MOVE GIZMO (the coloured arrows) on top of it.
 - You can add a SECONDARY POINT by holding CONTROL and clicking on a point while another one is already selected.
      - This will allow you to mainpulate the point you just selected by itself while showing stats relative to the first point.
 - You can ADD any number of points by holding SHIFT and clicking on them.
      - This will allow you to move ALL points selected.
 - You can DESELECT ALL POINTS by clicking in empty space.

 - Move though your viewport by holding the MIDDLE MOUSE BUTTON and the moving the mouse.
 - Zoom in and out of the viewport by using the SCROLL WHEEL.)");
}