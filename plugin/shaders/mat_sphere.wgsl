//===================================
//Sphere Shader
//===================================
fn fragmentSphere (in: VertexOutput) -> vec4f {
    let colorMod = (u.pressed * 0.3) + 0.05;

    let baseColor = vec3f(colorMod, 0.07, 1.0);
    return vec4f(baseColor, 1.0);
}

fn vertexSphere (position: ptr<function, vec3f>) -> vec4f {
    let wave = sin(u.time * u.pressed);
    position.x -= 3.0;
    position.y -= 1.7;
    position.x *= 0.25;
    position.y *= 0.4;
    return vec4f(*position, 1.0);

}