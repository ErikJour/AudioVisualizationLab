//===================================
//Sphere Shader
//===================================
fn fragmentSphere (in: VertexOutput) -> vec4f {
    let baseColor = vec3f(0.05, 0.07, 1.0);
    return vec4f(baseColor, 1.0);
}

fn vertexSphere (position: ptr<function, vec3f>) -> vec4f {
    let wave = sin(u.time);
    position.x -= 2.5;
    position.y -= 1.3;
    position.x *= 0.3;
    position.y *= 0.5;
    return vec4f(*position, 1.0);

}