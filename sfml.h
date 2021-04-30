#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
const int   width = 1620,  height = 1080,
          i_width = 300, i_height = 1080;

Shader shader;
RenderWindow window   (VideoMode(  width,   height), "Raytrace");
RenderWindow interface(VideoMode(i_width, i_height), "Interface");