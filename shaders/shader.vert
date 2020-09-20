#version 100

precision mediump float;
attribute vec3 a_position;
attribute vec4 a_color;
varying vec4 v_color;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * vec4(a_position, 1.0);
    v_color = a_color;
}