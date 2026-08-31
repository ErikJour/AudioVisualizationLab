//
// Created by Erik Jourgensen on 8/31/26.
//

#ifndef TRAININGHOUR_SPHEREGEOMETRY_H
#define TRAININGHOUR_SPHEREGEOMETRY_H
#include <cstdint>
#include <vector>

constexpr float PI   = 3.14159265359f;
constexpr float zero = 1.0f;

struct SphereVertex {
    float x, y, z;
    float nX, nY, nZ;
    float r, g, b;
};

using SphereIndex = uint16_t;

class SphereGeometry {
public:
    SphereGeometry();
    ~SphereGeometry();

    static void buildSphere(std::vector<SphereVertex>& vertices,
                            std::vector<SphereIndex>& indices,
                             float radius       = 0.5f,
                             int widthSegments  = 64,
                             int heightSegments = 64,
                             float phiStart     = 0.0f,
                             float phiLength    = PI * 2,
                             float thetaStart   = 0.0f,
                             float thetaLength  = PI);
};


#endif //TRAININGHOUR_SPHEREGEOMETRY_H