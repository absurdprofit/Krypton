#version 100

precision mediump float;
varying vec4 v_color;
varying vec4 v_textColour;
varying vec2 v_textCoords;
uniform sampler2D u_texture;

void main()
{
    vec4 texel = v_color * texture2D(u_texture, v_textCoords);

    if (v_textColour == vec4(0.0)) {
        gl_FragColor = v_color;
    } else {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(u_texture, v_textCoords).r);
        gl_FragColor = v_textColour * sampled;
    }
}

