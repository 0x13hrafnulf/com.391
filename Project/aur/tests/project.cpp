#include "aur.hpp"
#include "chrono"

#include "Player.hpp"
#include "Monster.hpp"
#include "Decoration.hpp"
static const float CAMERA_SPEED{0.3f};
static const float CAMERA_SENSITIVITY{0.003f};

const glm::vec4 FORWARD{0.0f, 0.0f, 1.0f, 0.0f};
const glm::vec4 RIGHT{1.0f, 0.0f, 0.0f, 0.0f};

[[noreturn]]
int main(int argc, char **argv)
{
    using namespace aur;

    // Window

    auto window = std::make_shared<SDLWindow>("aur", 1280, 768);
    window->set_capture_mouse_enabled(true);
    window->set_relative_mouse_mode_enabled(true);

    // Room Ground
    Decoration floor{ glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(-M_PI / 2.0f, 0.0f, 0.0f),  50, 50, Decoration::Type::Floor, 10, 10, "data/images/CEIL1_2.png", 1 };
    Decoration wall1{ glm::vec3(0.0f, 5.0f, -25.0f), glm::vec3(0.0f, 0.0f, 0.0f),  50, 15, Decoration::Type::Wall, 10, 10, "data/images/STARG3.png", 1 };
    Decoration wall2{ glm::vec3(0.0f, 5.0f, 25.0f), glm::vec3(-0.0f, M_PI, 0.0f),  50, 15, Decoration::Type::Wall, 10, 10, "data/images/STARG3.png", 1 };
    Decoration wall3{ glm::vec3(25.0f, 5.0f, 0.0f), glm::vec3(0.0f, -M_PI / 2.0f, 0.0f),  50, 15, Decoration::Type::Wall, 10, 10, "data/images/STARG3.png", 1 };
    Decoration wall4{ glm::vec3(-25.0f, 5.0f, 0.0f), glm::vec3(0.0f, M_PI / 2.0f, 0.0f),  50, 15, Decoration::Type::Wall, 10, 10, "data/images/STARG3.png", 1 };

    // Monsters & Player

    Monster monster1{ glm::vec3(-15.0f, 2.0f, -15.0f), 9, 0.5f, "data/images/TROOA1.png", 15, 4, 7};
    Monster monster2{ glm::vec3(15.0f, 2.0f, -15.0f), 9, 0.5f, "data/images/TROOA1.png", 15, 4, 7};
    Monster monster3{ glm::vec3(0.0f, 2.0f, -15.0f), 9, 0.5f, "data/images/TROOA1.png", 15, 4, 7 };

    Player player{glm::vec3(0.0f, -2.0f, 0.0f), 2, "data/images/SHTGB0.png", 6, 1};


    // Lamps

    auto[lamp_indices, lamp_vertices] = geometry_generators::generate_sphere_geometry_data(0.2f, 20, 20);
    auto lamp_sphere_geometry = std::make_shared<ES2Geometry>(lamp_indices, lamp_vertices);
    auto lamp_material = std::make_shared<ES2ConstantMaterial>();
    auto lamp1 = std::make_shared<Mesh>(lamp_sphere_geometry, lamp_material);
    auto lamp2 = std::make_shared<Mesh>(lamp_sphere_geometry, lamp_material);
    auto lamp3 = std::make_shared<Mesh>(lamp_sphere_geometry, lamp_material);
    auto lamp4 = std::make_shared<Mesh>(lamp_sphere_geometry, lamp_material);

    std::vector<std::shared_ptr<Object>> objects{ floor.get_mesh(), monster1.get_mesh(), monster2.get_mesh(), monster3.get_mesh(), wall1.get_mesh(), wall2.get_mesh(), wall3.get_mesh(), wall4.get_mesh(), player.get_mesh()};
    auto scene = std::make_shared<Scene>(objects);

    // Point Lights

    auto point_light1 = std::make_shared<PointLight>();
    point_light1->set_intensity(4000.0f);
    point_light1->set_constant_attenuation(0.0f);
    point_light1->set_linear_attenuation(0.2f);
    point_light1->set_quadratic_attenuation(0.8f);
    point_light1->set_two_sided(true);
    point_light1->set_position(glm::vec3(-25.0f, 4.0f, 25.0f));
    point_light1->add_child(lamp1);
    scene->get_root()->add_child(point_light1);
    scene->get_point_lights().push_back(point_light1);

    auto point_light2 = std::make_shared<PointLight>();
    point_light2->set_intensity(4000.0f);
    point_light2->set_constant_attenuation(0.0f);
    point_light2->set_linear_attenuation(0.2f);
    point_light2->set_quadratic_attenuation(0.8f);
    point_light2->set_two_sided(true);
    point_light2->set_position(glm::vec3(25.0f, 4.0f, 25.0f));
    point_light2->add_child(lamp2);
    scene->get_root()->add_child(point_light2);
    scene->get_point_lights().push_back(point_light2);

    auto point_light3 = std::make_shared<PointLight>();
    point_light3->set_intensity(4000.0f);
    point_light3->set_constant_attenuation(0.0f);
    point_light3->set_linear_attenuation(0.2f);
    point_light3->set_quadratic_attenuation(0.8f);
    point_light3->set_two_sided(true);
    point_light3->set_position(glm::vec3(25.0f, 4.0f, -25.0f));
    point_light3->add_child(lamp3);
    scene->get_root()->add_child(point_light3);
    scene->get_point_lights().push_back(point_light3);

    auto point_light4 = std::make_shared<PointLight>();
    point_light4->set_intensity(4000.0f);
    point_light4->set_constant_attenuation(0.0f);
    point_light4->set_linear_attenuation(0.2f);
    point_light4->set_quadratic_attenuation(0.8f);
    point_light4->set_two_sided(true);
    point_light4->set_position(glm::vec3(-25.0f, 4.0f, -25.0f));
    point_light4->add_child(lamp4);
    scene->get_root()->add_child(point_light4);
    scene->get_point_lights().push_back(point_light4);

    // Camera

    auto camera = scene->get_camera();
    camera->set_position(glm::vec3(0.0f, 0.0f, 20.0f));
    camera->set_zoom(3.0f);

    // Input

    window->set_on_key_down([&](int key) {
        glm::mat4 model_matrix = camera->get_model_matrix();
        glm::mat2x3 position = { glm::vec3(model_matrix * FORWARD * CAMERA_SPEED), glm::vec3(model_matrix * RIGHT * CAMERA_SPEED) };
        position[0][1] = 0.0f;
        position[1][1] = 0.0f;
        bool check_position = player.check_position(std::make_shared<Decoration>(floor));
            switch (key) {
            case SDLK_w: camera->add_to_position(-position[0]); break;
            case SDLK_a: camera->add_to_position(-position[1]); break;
            case SDLK_s: camera->add_to_position(position[0]); break;
            case SDLK_d: camera->add_to_position(position[1]); break;
                //case SDLK_SPACE: camera->add_to_y(CAMERA_SPEED); break;
            case SDLK_ESCAPE: exit(0);
            default: break;
        }
 
    });
    window->set_on_key_pressed([&](const Uint8* keystates) {
        glm::mat4 model_matrix = camera->get_model_matrix();
        glm::mat2x3 position = { glm::vec3(model_matrix * FORWARD * CAMERA_SPEED), glm::vec3(model_matrix * RIGHT * CAMERA_SPEED) };
        position[0][1] = 0.0f;
        position[1][1] = 0.0f;
        bool check_position = player.check_position(std::make_shared<Decoration>(floor));
            if (keystates[SDL_SCANCODE_W] && check_position)
            {
                camera->add_to_position(-position[0]);
            }
            if (keystates[SDL_SCANCODE_A] && check_position)
            {
                camera->add_to_position(-position[1]);
            }
            if (keystates[SDL_SCANCODE_S] && check_position )
            {
                camera->add_to_position(position[0]);
            }
            if (keystates[SDL_SCANCODE_D] && check_position)
            {
                camera->add_to_position(position[1]);
            }
            if (keystates[SDL_SCANCODE_SPACE])
            {

            }
            if (keystates[SDL_SCANCODE_ESCAPE])
            {
                exit(0);
            }
    });
    window->set_on_mouse_move([&](int x, int y, int x_rel, int y_rel) {

        camera->add_to_rotation_y(static_cast<float>(-x_rel)* CAMERA_SENSITIVITY);
        camera->add_to_rotation_x(static_cast<float>(-y_rel)* CAMERA_SENSITIVITY);
 
    });

    window->set_on_mouse_down([&](int button, int x, int y)
    {
        int cx = static_cast<int>(window->get_width() / 2);
        int cy = static_cast<int>(window->get_height() / 2);
        Ray ray = camera->world_ray_from_screen_point(cx, cy, static_cast<int>(window->get_width()), static_cast<int>(window->get_height()));
        
        if (player.get_state() == Agent::Alive)
        {
            player.shoot();
            if (monster1.intersects_with_ray(ray)) {
                monster1.kill();
            }
            if (monster2.intersects_with_ray(ray)) {
                monster2.kill();
            }
            if (monster3.intersects_with_ray(ray)) {
                monster3.kill();
            }
        }    
    });
    window->set_on_button_pressed([&](bool state) {

        if (state == SDL_PRESSED){
            int cx = static_cast<int>(window->get_width() / 2);
            int cy = static_cast<int>(window->get_height() / 2);
            Ray ray = camera->world_ray_from_screen_point(cx, cy, static_cast<int>(window->get_width()), static_cast<int>(window->get_height()));

            if (player.get_state() == Agent::Alive)
            {
                player.shoot();
                if (monster1.intersects_with_ray(ray)) {
                    monster1.kill();
                }
                if (monster2.intersects_with_ray(ray)) {
                    monster2.kill();
                }
                if (monster3.intersects_with_ray(ray)) {
                    monster3.kill();
                }
            }
        }
    });


    player.set_camera(camera);
    monster1.set_target(std::make_shared<Player>(player));
    monster2.set_target(std::make_shared<Player>(player));
    monster3.set_target(std::make_shared<Player>(player));
    // Rendering
    auto previous_frame_time = std::chrono::steady_clock::now();
    ES2Renderer renderer(scene, window);
    for (;;) {
        window->key_pressed();
        window->button_pressed();
        window->poll();

        auto current_frame_time = std::chrono::steady_clock::now();
        auto frame_time_diff = current_frame_time - previous_frame_time;
        float delta_time = frame_time_diff.count();

        monster1.update(delta_time);
        monster2.update(delta_time);
        monster3.update(delta_time);
        player.update(delta_time);

        renderer.render();
    }
}

/*
auto[room_ground_indices, room_ground_vertices] = geometry_generators::generate_plane_geometry_data(50, 50, 1, 1);
    auto room_ground_geometry = std::make_shared<ES2Geometry>(room_ground_indices, room_ground_vertices);
    auto room_ground_material = std::make_shared<ES2PhongMaterial>();
    auto [image_r1_data, image_r1_width, image_r1_height, image_r1_channels] = file_utilities::read_image_file("data/images/CEIL1_2.png");
    auto texture_r = std::make_shared<ES2Texture>(image_r1_data, image_r1_width, image_r1_height, image_r1_channels);
    texture_r->set_enabled(true);
    texture_r->set_wrap_mode_s(Texture::WrapMode::Repeat);
    texture_r->set_wrap_mode_t(Texture::WrapMode::Repeat);
    glm::mat4 texture_r_transformation_matrix { 1.0f };
    texture_r_transformation_matrix[0][0] = 10.0f;
    texture_r_transformation_matrix[1][1] = 10.0f;
    texture_r->set_transformation_enabled(true);
    texture_r->set_transformation_matrix(texture_r_transformation_matrix);
    room_ground_material->set_texture_1(texture_r);

    room_ground_material->set_specular_exponent(1.0f);
    room_ground_material->set_specular_color(glm::vec3{0.0f});
    room_ground_material->set_diffuse_color(glm::vec4{0.25f});
    auto room_ground = std::make_shared<Mesh>(room_ground_geometry, room_ground_material);
    room_ground->set_position(glm::vec3(0.0f, -2.5f, 0.0f));
    room_ground->set_rotation(glm::vec3(-M_PI / 2.0f, 0.0f, 0.0f));

    // Walls
    auto [image_w_data, image_w_width, image_w_height, image_w_channels] = file_utilities::read_image_file("data/images/SKULWALL.png");
    auto texture_w = std::make_shared<ES2Texture>(image_w_data, image_w_width, image_w_height, image_w_channels);
    texture_w->set_enabled(true);
    texture_w->set_wrap_mode_s(Texture::WrapMode::Repeat);
    texture_w->set_wrap_mode_t(Texture::WrapMode::Repeat);
    glm::mat4 texture_w_transformation_matrix{ 1.0f };
    texture_w_transformation_matrix[0][0] = 10.0f;
    texture_w_transformation_matrix[1][1] = 10.0f;
    texture_w->set_transformation_enabled(true);
    texture_w->set_transformation_matrix(texture_w_transformation_matrix);




    auto [wall_indices_1, wall_vertices_1] = geometry_generators::generate_plane_geometry_data(50, 15, 1, 1);
    auto wall_geometry_1 = std::make_shared<ES2Geometry>(wall_indices_1, wall_vertices_1);
    auto wall_material_1 = std::make_shared<ES2PhongMaterial>();
    wall_material_1->set_texture_1(texture_w);
    wall_material_1->set_specular_exponent(1.0f);
    wall_material_1->set_specular_color(glm::vec3{ 0.0f });
    wall_material_1->set_diffuse_color(glm::vec4{ 0.25f });
    auto wall_1 = std::make_shared<Mesh>(wall_geometry_1, wall_material_1);
    wall_1->set_position(glm::vec3(0.0f, 5.0f, -25.0f));
    wall_1->set_rotation(glm::vec3(0.0f, 0.0f, 0.0f));


    auto [wall_indices_2, wall_vertices_2] = geometry_generators::generate_plane_geometry_data(50, 15, 1, 1);
    auto wall_geometry_2 = std::make_shared<ES2Geometry>(wall_indices_2, wall_vertices_2);
    auto wall_material_2 = std::make_shared<ES2PhongMaterial>();
    wall_material_2->set_texture_1(texture_w);
    wall_material_2->set_specular_exponent(1.0f);
    wall_material_2->set_specular_color(glm::vec3{ 0.0f });
    wall_material_2->set_diffuse_color(glm::vec4{ 0.25f });
    auto wall_2 = std::make_shared<Mesh>(wall_geometry_2, wall_material_2);
    wall_2->set_position(glm::vec3(0.0f, 5.0f, 25.0f));
    wall_2->set_rotation(glm::vec3(0.0f, M_PI , 0.0f));


    auto [wall_indices_3, wall_vertices_3] = geometry_generators::generate_plane_geometry_data(50, 15, 1, 1);
    auto wall_geometry_3 = std::make_shared<ES2Geometry>(wall_indices_3, wall_vertices_3);
    auto wall_material_3 = std::make_shared<ES2PhongMaterial>();
    wall_material_3->set_texture_1(texture_w);
    wall_material_3->set_specular_exponent(1.0f);
    wall_material_3->set_specular_color(glm::vec3{ 0.0f });
    wall_material_3->set_diffuse_color(glm::vec4{ 0.25f });
    auto wall_3 = std::make_shared<Mesh>(wall_geometry_3, wall_material_3);
    wall_3->set_position(glm::vec3(25.0f, 5.0f, 0.0f));
    wall_3->set_rotation(glm::vec3(0.0f, -M_PI / 2.0f, 0.0f));


    auto [wall_indices_4, wall_vertices_4] = geometry_generators::generate_plane_geometry_data(50, 15, 1, 1);
    auto wall_geometry_4 = std::make_shared<ES2Geometry>(wall_indices_4, wall_vertices_4);
    auto wall_material_4 = std::make_shared<ES2PhongMaterial>();
    wall_material_4->set_texture_1(texture_w);
    wall_material_4->set_specular_exponent(1.0f);
    wall_material_4->set_specular_color(glm::vec3{ 0.0f });
    wall_material_4->set_diffuse_color(glm::vec4{ 0.25f });
    auto wall_4 = std::make_shared<Mesh>(wall_geometry_4, wall_material_4);
    wall_4->set_position(glm::vec3(-25.0f, 5.0f, 0.0f));
    wall_4->set_rotation(glm::vec3(0.0f, M_PI / 2.0f, 0.0f));
*/