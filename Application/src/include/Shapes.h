//
// Created by ankit on 9/12/20.
//

#ifndef OPENGL_CORE_SHAPES_H
#define OPENGL_CORE_SHAPES_H

#include "core.h"
#include "Drawable.h"

namespace cube
{
        const std::vector<vec3> pos{
            //up
             vec3(-1, 1, 1),	 //0
             vec3(1, 1, 1),	 //1
             vec3(1, 1, -1),	 //2
             vec3(-1, 1, -1), //3
             //down
             vec3(-1, -1, 1),  //4
             vec3(1, -1, 1),	  //5
             vec3(1, -1, -1),  //6
             vec3(-1, -1, -1), //7
        };
        const std::vector<Indexdata> indices{
            {{0, 1, 2, 3}, vec4(1, 0, 0), {vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1)}, 1}, //up
            {{7, 6, 5, 4}, vec4(1, 0, 0), {vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1)}, 1}, //down
            {{1, 0, 4, 5}, vec4(0, 1, 0), {vec2(1, 1), vec2(0, 1), vec2(0, 0), vec2(1, 0)}, 1}, //front
            {{3, 2, 6, 7}, vec4(0, 1, 0), {vec2(1, 1), vec2(0, 1), vec2(0, 0), vec2(1, 0)}, 1}, //back
            {{2, 1, 5, 6}, vec4(0, 0, 1), {vec2(1, 1), vec2(0, 1), vec2(0, 0), vec2(1, 0)}, 1}, //left
            {{0, 3, 7, 4}, vec4(0, 0, 1), {vec2(1, 1), vec2(0, 1), vec2(0, 0), vec2(1, 0)}, 1}	//right
        };
        const std::vector<uint32_t> index{
            0, 1, 2,
            0, 2, 3,
            7, 6, 5,
            7, 5, 4,
            1, 0, 4,
            1, 4, 5,
            3, 2, 6,
            3, 6, 7,
            2, 1, 5,
            2, 5, 6,
            0, 3, 7,
            0, 7, 4
        };
}


#endif //OPENGL_CORE_SHAPES_H
