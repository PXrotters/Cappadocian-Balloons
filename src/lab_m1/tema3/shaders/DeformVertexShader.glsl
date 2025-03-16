#version 330 core

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output to Fragment Shader
out vec3 frag_normal;
out vec2 frag_texcoord;

void main()
{
    vec3 modified_position = v_position;

    // Deformarea doar pentru jumatatea inferioara (y intre 0 si -0.5)
    if (v_position.y <= 0.0 && v_position.y >= -0.5) {
        // Calculam un factor de scalare bazat pe distanta fata de y = 0
        float scale_factor = 1.0 + abs(v_position.y) * 2.0; // Creste coordonata Y proportional cu |y|
        modified_position.y *= scale_factor;  // Aplicam deformarea pe axa Y
    }

    // Transformarile standard
    frag_normal = mat3(Model) * v_normal;
    frag_texcoord = v_texcoord;

    // Calculati pozitia finala
    gl_Position = Projection * View * Model * vec4(modified_position, 1.0);
}
