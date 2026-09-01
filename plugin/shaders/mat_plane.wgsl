//===================================
//Plane Shader
//===================================
fn fragmentPlane (in: VertexOutput) -> vec4f {
    let baseColor = vec3f(0.5 /** u.time*/, 0.4 * u.time, 1.0 * u.time);
    return vec4f(baseColor, 1.0);
}

fn vertexPlane (position: ptr<function, vec3f>) -> vec4f {
    let wave = sin(u.time);
    return vec4f(*position, 1.0);

}