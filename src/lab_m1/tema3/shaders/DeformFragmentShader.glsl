#version 330 core

// Input from Vertex Shader
in vec2 frag_texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int flag;

// Output color
out vec4 out_color;

void main()
{
    vec4 color1 = texture(texture_1, frag_texcoord);
    vec4 color2 = texture(texture_2, frag_texcoord);

    if (flag == 1) {
        out_color = mix(color1, color2, 0.5);
    } else {
        out_color = color1;
    }

    // Ignora pixelii transparenti
    if (out_color.a < 0.5) {
        discard;
    }
}
