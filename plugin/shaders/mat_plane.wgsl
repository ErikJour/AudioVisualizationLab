//===================================
//Plane Shader
//===================================
fn fragmentPlane (in: VertexOutput) -> vec4f {
//    let red = sin(u.time);
    let baseColor = vec3f(0.3, 0.4, 0.0);
    return vec4f(baseColor, 1.0);
}

fn vertexPlane (position: ptr<function, vec3f>) -> vec4f {

    return vec4f(*position, 1.0);

}