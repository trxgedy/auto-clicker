module;

#include "imgui\imgui.h"

export module Widgets;

export namespace custom
{
    void rainbow_rect( ImVec2 position, float width, float height )
    {
        ImColor colors[ ] = {
            ImColor( 255, 0, 0, 255 ),
            ImColor( 255, 165, 0, 255 ),
            ImColor( 255, 255, 0, 255 ),
            ImColor( 0, 255, 0, 255 ),
            ImColor( 0, 0, 255, 255 ),
            ImColor( 75, 0, 130, 255 ),
        };

        float segment = width / 5;

        for ( int i = 0; i < 5; ++i )
        {
            ImVec2 start = { position.x + i * segment, position.y };
            ImVec2 end = { start.x + segment, position.y + height };

            ImGui::GetWindowDrawList( )->AddRectFilledMultiColor( start, end, colors[ i ], colors[ i + 1 ], colors[ i + 1 ], colors[ i ] );
        }
    }
}