//
// Created by Erik Jourgensen on 7/30/26.
//

#pragma once
#include <cstdint>
#include <vector>

struct PlaneVertex {

	float  x,  y,  z; //Position
	float nX, nY, nZ; //Normal
	float  r,  g,  b; //Color

};

using planeIndex = uint16_t;



class PlaneGeometry {
public:
	static void buildPlane(std::vector<PlaneVertex>& vertices, std::vector<planeIndex>& indices, int width, int height, int widthSegments, int heightSegments);

};



