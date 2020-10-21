#include "aur.hpp"

static const float CAMERA_SPEED{0.1f};
static const float CAMERA_ROT_SPEED{0.05f};
static const float PLANE_ROT_SPEED{0.001f};

const glm::vec4 FORWARD{0.0f, 0.0f, 5.0f, 0.0f};

[[noreturn]]
int main(int argc, char **argv)
{
    using namespace aur;

    auto window = std::make_shared<SDLWindow>("aur", 1280, 768);

    auto[plane1_indices, plane1_vertices] = geometry_generators::generate_plane_geometry_data(100, 100, 2, 2);
    auto plane1_geometry = std::make_shared<ES2Geometry>(plane1_indices, plane1_vertices);
    auto plane1_material = std::make_shared<ES2PhongMaterial>();
    auto[image1_data, image1_width, image1_height, image1_channels] = file_utilities::read_image_file("../data/images/brickwall.jpg");
    auto[image2_data, image2_width, image2_height, image2_channels] = file_utilities::read_image_file("../data/images/brickwall_n.jpg");
    auto texture1 = std::make_shared<ES2Texture>(image1_data, image1_width, image1_height, image1_channels);
    auto texture1_normal = std::make_shared<ES2Texture>(image2_data, image2_width, image2_height, image2_channels);
    plane1_material->set_texture_1(texture1);
    plane1_material->set_texture_1_normal(texture1_normal);
    plane1_material->set_specular_exponent(20.0f);
    auto plane1 = std::make_shared<Mesh>(plane1_geometry, plane1_material);
    plane1->set_y(-10);
    plane1->set_rotation_x(-M_PI / 2);

    std::vector<std::shared_ptr<Object>> objects{plane1};
    auto scene = std::make_shared<Scene>(objects);
    auto root = scene->get_root();

 
    auto point_light1 = std::make_shared<PointLight>();
    auto point_light2 = std::make_shared<PointLight>();
    point_light1->set_intensity(0.86f);
    point_light1->set_z(-10.0f);
    point_light1->set_y(2.0f);
    point_light1->set_two_sided(true);
    point_light2->set_intensity(0.75f);
    point_light1->set_z(-10.0f);
    point_light2->set_y(2.0f);
    point_light2->set_two_sided(true);
    root->add_child(point_light1);
    root->add_child(point_light2);
    scene->get_point_lights().push_back(point_light1);
    scene->get_point_lights().push_back(point_light2);

    auto camera = scene->get_camera();
    camera->set_z(40.0f);
    camera->set_y(70.0f);
    camera->set_rotation_x(-M_PI / 2.5);

    window->set_on_key([&](int key) {
        switch (key) {
            case SDLK_w: camera->add_to_rotation_x(-CAMERA_ROT_SPEED); break;
            case SDLK_a: camera->add_to_rotation_y(CAMERA_ROT_SPEED); break;
            case SDLK_s: camera->add_to_rotation_x(CAMERA_ROT_SPEED); break;
            case SDLK_d: camera->add_to_rotation_y(-CAMERA_ROT_SPEED); break;
            case SDLK_e: camera->add_to_y(CAMERA_ROT_SPEED); break;
            case SDLK_q: camera->add_to_y(-CAMERA_ROT_SPEED); break;
            case SDLK_UP: camera->add_to_position(-glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED)); break;
            case SDLK_DOWN: camera->add_to_position(glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED)); break;
            case SDLK_ESCAPE: exit(0);
            default: break;
        }
    });

    bool plane1_rotation_enabled{false};
    glm::vec4 first_diffuse_color{plane1_material->get_diffuse_color()};
    bool texture1_enabled{true};
    Texture::Mode texture1_texturing_mode{Texture::Mode::Modulation};
    Texture::FilterType texture1_magnification_filter_type{Texture::FilterType::Nearest};
    Texture::FilterType texture1_minification_filter_type{Texture::FilterType::Nearest};
    Texture::WrapMode texture1_wrap_mode_s{Texture::WrapMode::Repeat};
    Texture::WrapMode texture1_wrap_mose_t{Texture::WrapMode::Repeat};
    bool texture1_transformation_matrix_enabled{false};
    glm::mat4 texture1_transformation_matrix{1.0f};
    int texture1_anisotropy{0};
    bool texture1_mipmaps_enabled{true};
    bool texture1_normal_enabled{false};


    ES2Renderer renderer(scene, window);
    int i = 0;
    for (;;) {
        window->poll();

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin("Parameters", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
        {
            if (ImGui::CollapsingHeader("Texture 1 Parameters ")) {
                ImGui::Checkbox("Rotate##1", &plane1_rotation_enabled);
                ImGui::InputFloat4("Emission Color##1", glm::value_ptr(first_diffuse_color), "%.2f");
                ImGui::Checkbox("Enabled##1", &texture1_enabled);
                ImGui::Combo(
                    "Texturing Mode##1", reinterpret_cast<int *>(&texture1_texturing_mode),
                    "Addition\0Subtraction\0ReverseSubtraction\0Modulation\0Decaling\0"
                );
                ImGui::Combo(
                    "First Coordinate Wrap Mode##1", reinterpret_cast<int *>(&texture1_wrap_mode_s),
                    "Repeat\0Mirrored Repeat\0Clamp To Edge\0"
                );
                ImGui::Combo(
                    "Second Coordinate Wrap Mode##1", reinterpret_cast<int *>(&texture1_wrap_mose_t),
                    "Repeat\0Mirrored Repeat\0Clamp To Edge\0"
                );
                ImGui::Combo(
                    "Magnification Filter##1", reinterpret_cast<int *>(&texture1_magnification_filter_type),
                    "Nearest\0Linear\0Nearest Mipmap Nearest\0Nearest Mipmap Linear\0Linear Mipmap Nearest\0Linear Mipmap Linear\0"
                );
                ImGui::Combo(
                    "Minification Filter##1", reinterpret_cast<int *>(&texture1_minification_filter_type),
                    "Nearest\0Linear\0Nearest Mipmap Nearest\0Nearest Mipmap Linear\0Linear Mipmap Nearest\0Linear Mipmap Linear\0"
                );
                ImGui::Checkbox("Enable Transformation Matrix##1", &texture1_transformation_matrix_enabled);
                ImGui::InputFloat4("##1", glm::value_ptr(texture1_transformation_matrix), "%.2f");
                ImGui::InputFloat4("##2", glm::value_ptr(texture1_transformation_matrix) + 4, "%.2f");
                ImGui::InputFloat4("##3", glm::value_ptr(texture1_transformation_matrix) + 8, "%.2f");
                ImGui::InputFloat4("##4", glm::value_ptr(texture1_transformation_matrix) + 12, "%.2f");
                ImGui::SliderInt("Anisotropy##1", &texture1_anisotropy, 0, 16);
                ImGui::Checkbox("Use mipmaps##1", &texture1_mipmaps_enabled);
                ImGui::Checkbox("Normal Mapping", &texture1_normal_enabled);
            }
        }
        ImGui::End();

        if (plane1_rotation_enabled) {
            plane1->add_to_rotation_y(PLANE_ROT_SPEED);
        }
        plane1_material->set_diffuse_color(first_diffuse_color);
        texture1->set_enabled(texture1_enabled);
        texture1->set_mode(texture1_texturing_mode);
        texture1->set_wrap_mode_s(texture1_wrap_mode_s);
        texture1->set_wrap_mode_t(texture1_wrap_mose_t);
        texture1->set_magnification_filter(texture1_magnification_filter_type);
        texture1->set_minification_filter(texture1_minification_filter_type);
        texture1->set_transformation_enabled(texture1_transformation_matrix_enabled);
        texture1->set_transformation_matrix(texture1_transformation_matrix);
        texture1->set_anisotropy(static_cast<float>(texture1_anisotropy));
        texture1->set_mipmaps_enabled(texture1_mipmaps_enabled);
        texture1_normal->set_enabled(texture1_normal_enabled);

        float angle = (float)i / (float)200 * 2 * M_PI;
        float x = cosf(angle) * 20.0f;
        float z = sinf(angle) * 20.0f;
        // TODO
        point_light1->set_position(glm::vec3{ x, point_light1->get_y(), z });
        point_light2->set_position(glm::vec3{ x, point_light2->get_y(), -z });

        i = i % 200 + 1;

        renderer.render();
    }
}
