//
// Created by Erik Jourgensen on 7/30/26.
//

#include "PlaneGeometry.h"

void PlaneGeometry::buildPlane(std::vector<PlaneVertex>& vertices, std::vector<planeIndex>& indices,
												const int width, const int height,
												const int widthSegments, const int heightSegments)
{
		//=========================================
		//Get half widths and height
	    //=========================================
		float widthHalf  = static_cast<float>(width) / 2.0f;
		float heightHalf = static_cast<float>(height) / 2.0f;
		//=========================================
		//Get grid x and y values using floor function
		//=========================================
		int gridX = widthSegments;
		int gridY = heightSegments;
		int gridX1 = gridX + 1;
		int gridY1 = gridY + 1;

		const float segmentWidth  = static_cast<float>(width)  / static_cast<float>(gridX);
		const float segmentHeight = static_cast<float>(height) / static_cast<float>(gridY);

		//=========================================
		//Set up our grid
		//=========================================

		std::vector<uint32_t> grid;
		grid.reserve((static_cast<size_t>(heightSegments) + 1) * (static_cast<size_t>(widthSegments) + 1));

		for ( int iy = 0; iy < gridY1; iy++ ) {

			float y = static_cast<float>(iy) * segmentHeight - heightHalf;

			for ( int ix = 0; ix < gridX1; ix ++ ) {

				float x = static_cast<float>(ix) * segmentWidth - widthHalf;

				vertices.push_back(
									{ x, -y, 0.0f,
									0.0f, 0.0f, 1.0f,
									1.0f, 1.0f, 1.0f }
									);
			}
		}

		for ( int iy = 0; iy < gridY; iy ++ ) {

			for ( int ix = 0; ix < gridX; ix ++ ) {

				int a = ix + gridX1 * iy;
				int b = ix + gridX1 * ( iy + 1 );
				int c = ( ix + 1 ) + gridX1 * ( iy + 1 );
				int d = ( ix + 1 ) + gridX1 * iy;

				indices.push_back(static_cast<uint16_t>(a));
				indices.push_back(static_cast<uint16_t>(b));
				indices.push_back(static_cast<uint16_t>(d));
				indices.push_back(static_cast<uint16_t>(b));
				indices.push_back(static_cast<uint16_t>(c));
				indices.push_back(static_cast<uint16_t>(d));
			}

		}

}
