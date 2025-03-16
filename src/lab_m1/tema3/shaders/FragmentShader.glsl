#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int flag;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 colourX = texture2D(texture_1, texcoord);
	vec4 colourY = texture2D(texture_2, texcoord);

	if (flag == 1) {
        vec4 colourY = texture2D(texture_2, texcoord);
        out_color = mix(colourX, colourY, 0.5f);
    } else {
        out_color = colourX;
    }

    if (out_color.a < 0.5f) {
        discard;
    }


}
