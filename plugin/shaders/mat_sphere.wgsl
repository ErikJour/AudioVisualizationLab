//===================================
//Sphere Shader
//===================================
fn fragmentSphere (in: VertexOutput) -> vec4f {
    let baseColor = vec3f(0.05, 0.07, 1.0);
    return vec4f(baseColor, 1.0);
}

fn vertexSphere (position: ptr<function, vec3f>) -> vec4f {
    let wave = sin(u.time);
    position.x -= 3.0;
    position.y -= 1.7;
    position.x *= 0.25;
    position.y *= 0.4;
    return vec4f(*position, 1.0);

}