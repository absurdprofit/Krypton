#version 100

precision mediump float;
attribute vec3 a_position;
attribute vec4 a_color;
attribute vec2 a_textCoords;
attribute vec4 a_textColour;
varying vec4 v_textColour;
varying vec4 v_color;
varying vec2 v_textCoords;
uniform mat4 u_projectionMatrix;
// uniform sampler2D u_texture;

void main() {
    gl_Position = u_projectionMatrix * vec4(a_position, 1.0);
    v_color = a_color;
    v_textColour = a_textColour;
    v_textCoords = a_textCoords;
}