attribute vec4 position;
attribute vec4 color;
attribute vec3 normal;
attribute vec4 texture1_coordinates;
attribute vec4 texture2_coordinates;
attribute vec4 tangent;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;
uniform float point_size;

uniform bool texture1_enabled;
uniform bool texture1_transformation_enabled;
uniform mat4 texture1_transformation_matrix;

uniform sampler2D texture1_normal_sampler;
uniform bool texture1_normal_enabled;

uniform bool texture2_enabled;
uniform bool texture2_transformation_enabled;
uniform mat4 texture2_transformation_matrix;

varying vec3 fragment_view_direction;
varying vec3 fragment_view_normal;
varying vec4 fragment_color;
varying vec2 fragment_texture1_coordinates;
varying vec2 fragment_texture2_coordinates;
varying mat3 fragment_view_TBN;


void main()
{
    vec4 view_position = model_view_matrix * position;
    fragment_view_direction = -view_position.xyz;
    vec3 binormal = cross(normal, tangent.xyz) * tangent.w;
    fragment_view_normal = normalize(normal_matrix * normal);
    vec3 T = normalize(normal_matrix * tangent.xyz); 
    vec3 B = normalize(normal_matrix * binormal);
    vec3 N = normalize(normal_matrix * normal);
    fragment_view_TBN = mat3(T, B, N);

    fragment_color = color;
    if (texture1_enabled) {
        if (texture1_transformation_enabled) {
            vec4 transformed_texture1_coordinates = texture1_transformation_matrix * vec4(texture1_coordinates.st, 0.0, 1.0);
            fragment_texture1_coordinates = vec2(transformed_texture1_coordinates);
        } else {
            fragment_texture1_coordinates = vec2(texture1_coordinates);
        }
    }
    if (texture2_enabled) {
        if (texture2_transformation_enabled) {
            vec4 transformed_texture2_coordinates = texture2_transformation_matrix * vec4(texture2_coordinates.st, 0.0, 1.0);
            fragment_texture2_coordinates = vec2(transformed_texture2_coordinates);
        } else {
            fragment_texture2_coordinates = vec2(texture2_coordinates);
        }
    }


    gl_Position = projection_matrix * view_position;
    gl_PointSize = point_size;
}
