#version 120

attribute vec4 position;
attribute vec4 color;
attribute vec4 texture_coordinates;

uniform mat4 model_view_projection_matrix;
uniform vec4 emission_color;
uniform float point_size;
uniform float time;
varying vec4 fragment_color;
varying vec2 fragment_texture_coordinates;

void main()
{
    vec4 transformed = vec4(position);
    float dx = position.x;
    float dy = position.y;
    float freq = sqrt(dx*dx + dy*dy);
    float amp = 0.01;
    float angle = -time*10.0+freq*6.0;
    transformed.z += sin(angle)*amp;

    fragment_color = color * emission_color;
    fragment_texture_coordinates = vec2(texture_coordinates);
    gl_Position = model_view_projection_matrix * transformed;
    gl_PointSize = point_size;
}

