//
// Created by Erik Jourgensen on 8/31/26.
//

#include "SphereGeometry.h"

SphereGeometry::SphereGeometry() {}
SphereGeometry::~SphereGeometry() {}

void SphereGeometry::buildSphere(std::vector<SphereVertex>& vertices,
                            std::vector<SphereIndex>& indices,
                            const float radius       = 0.5f,
                            const int widthSegments  = 64,
                            const int heightSegments = 64,
                            const float phiStart     = 0.0f,
                            const float phiLength    = PI * 2,
                            const float thetaStart   = 0.0f,
                            const float thetaLength  = PI) {

        const float thetaEnd = std::min(thetaStart + thetaLength, PI);
        std::vector<uint32_t> grid;
        grid.reserve((static_cast<size_t>(heightSegments) + 1) * (static_cast<size_t>(widthSegments) + 1));
        SphereVertex vertex        = {};
        std::vector<float> normals = {};
        std::vector<float> uv      = {};

        for (int iy = 0; iy <= heightSegments; iy++) {
            std::vector<int> verticesRow = {};
            const float v             = static_cast<float>(iy) / static_cast<float>(heightSegments);
            const float inverse          = 1.0f / radius;

            for (int ix = 0; ix <= widthSegments; ix++) {
                const float u = static_cast<float>(ix) / static_cast<float>(widthSegments);
                vertex.x = -radius * std::cos(phiStart + u * phiLength) * std::sin(thetaStart + v * thetaLength);
                vertex.y = radius * std::cos(thetaStart + v * thetaLength);
                vertex.z = radius * std::sin(phiStart + u * phiLength) * std::sin(thetaStart + v * thetaLength);

                vertices.push_back( {
                vertex.x, vertex.y, vertex.z,
                vertex.x * inverse, vertex.y * inverse, vertex.z * inverse,
                zero, zero, zero});

                grid.push_back(static_cast<uint32_t>(vertices.size() - 1));
            }
        }

        for (int iy = 0; iy < heightSegments; iy++) {
            for (int ix = 0; ix < widthSegments; ix++) {
                const int rowWidth = widthSegments + 1;
                const auto a = grid[static_cast<uint32_t>(iy) * static_cast<uint32_t>(rowWidth) + static_cast<uint32_t>(ix + 1)];
                const auto b = grid[static_cast<uint32_t>(iy) * static_cast<uint32_t>(rowWidth) + static_cast<uint32_t>(ix)];
                const auto c = grid[static_cast<uint32_t>((iy + 1) * rowWidth + ix)];
                const auto d = grid[static_cast<uint32_t>((iy + 1) * rowWidth + (ix + 1))];

                if (iy != 0 || thetaStart > 0) {
                    indices.push_back(static_cast<uint16_t>(a));
                    indices.push_back(static_cast<uint16_t>(b));
                    indices.push_back(static_cast<uint16_t>(d));
                }

                if (iy != heightSegments - 1 || thetaEnd < PI) {
                    indices.push_back(static_cast<uint16_t>(b));
                    indices.push_back(static_cast<uint16_t>(c));
                    indices.push_back(static_cast<uint16_t>(d));
                }
            }
        }
    }