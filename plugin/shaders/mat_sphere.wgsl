//===================================
//Sphere Shader
//===================================
fn fragmentSphere (in: VertexOutput) -> vec4f {
    let baseColor = vec3f(0.5, 0.7, 1.0);
    return vec4f(baseColor, 1.0);
}

fn vertexSphere (position: ptr<function, vec3f>) -> vec4f {
    let wave = sin(u.time);
    position.z *= wave;
    position.y *= wave;
    position.x *= wave;
    return vec4f(*position, 1.0);

}