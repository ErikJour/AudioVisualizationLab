//
// Created by Erik Jourgensen on 8/31/26.
//

#ifndef TRAININGHOUR_SPHEREGEOMETRY_H
#define TRAININGHOUR_SPHEREGEOMETRY_H
#include <cstdint>
#include <vector>

constexpr float PI   = 3.14159265359f;
constexpr float zero = 0.0f;

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
                            const float radius,
                            int widthSegments,
                            int heightSegments,
                            const float phiStart,
                            const float phiLength,
                            const float thetaStart,
                            const float thetaLength);
};


#endif //TRAININGHOUR_SPHEREGEOMETRY_H