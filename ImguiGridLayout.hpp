#pragma once 

#include <imgui.h>
#include "imgui_impl_gf.h"

class GridLayout
{
    ImVec2 screen_size_;
    ImVec2 grid_division_;

    int menu_height_ { 20 };

public:
    bool active { true };

    GridLayout( 
        const ImVec2& screen_size,
        ImVec2 grid_division = ImVec2( 10, 10 ),
        int menu_height = 20 );

    void placeNextWindow( 
            const ImVec2& corner1, 
            const ImVec2& corner2, 
            const char* mode ,
            int border = 1 ) const;
    
    void resize( const ImVec2& screen_size ); 
};