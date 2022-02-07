#pragma once

#include "glm/glm.hpp"

namespace MKGame {
    enum CUBE_FACE {
        SOUTH = 0,         // +z
        NORTH = 1,         // -z
        EAST = 2,         // +x
        WEST = 3,         // -x
        TOP = 4,           // +y
        BOTTOM = 5,         // -y
        COUNT = 6,
        UP = TOP,
        DOWN = BOTTOM
    };

	class tile {
		virtual glm::ivec2 texture_offset(
            CUBE_FACE dir) const;
	};
}