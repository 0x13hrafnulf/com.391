attribute vec4 position;
attribute vec4 color;
attribute vec3 normal;
attribute vec4 texture_coordinates;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;
uniform float point_size;
uniform float time;

varying vec3 fragment_view_direction;
varying vec3 fragment_view_normal;
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

    vec4 view_position = model_view_matrix * transformed;
    fragment_view_direction = -view_position.xyz;
    fragment_view_normal = normalize(normal_matrix * normal);
    fragment_color = color;
    fragment_texture_coordinates = vec2(texture_coordinates);

    gl_Position = projection_matrix * view_position;
    gl_PointSize = point_size;
}
