#include "aur.hpp"
#include <ctime>
static const float CAMERA_SPEED{ 0.1f };
static const float CAMERA_ROT_SPEED{ 0.05f };
static const float CAMERA_MOUSE_ROT_SPEED{ 0.005f };
static const glm::vec4 FORWARD{ 0.0f, 0.0f, 1.0f, 0.0f };
static const glm::vec4 SIDE{ 1.0f, 0.0f, 0.0f, 0.0f };
[[ noreturn ]]
int main(int argc, char **argv)
{
    using namespace aur;

    auto window = std::make_shared<SDLWindow>("aur", 0, 0);

    // TODO
	auto plane_geometry_data = geometry_generators::generate_plane_geometry_data(1.0f, 1.0f, 50, 50);
	auto  plane_geometry = std::make_shared<ES2Geometry>(plane_geometry_data.first, plane_geometry_data.second);
	auto plane_material = std::make_shared<ES2PhongMaterial>();
	//plane_geometry->set_type(Geometry::Type::LineLoop);

	plane_material->set_emission_color(glm::vec4{ 0.05f, 0.05f, 0.55f, 1.0f });
	plane_material->set_specular_color(glm::vec3{ 0.5, 0.5, 0.5 });
	plane_material->set_specular_exponent(20.4f);
	plane_material->set_diffuse_color(glm::vec4{ 0.18275f, 0.17f, 0.22525f, 1.0f });
	plane_material->set_ambient_color(glm::vec3{ 0.05375f, 0.05f, 0.3f });

	auto sphere_geometry_data = geometry_generators::generate_sphere_geometry_data(0.25f, 30, 30);
	auto sphere_geometry = std::make_shared<ES2Geometry>(sphere_geometry_data.first, sphere_geometry_data.second);
	
	auto sphere_material = std::make_shared<ES2PhongMaterial>();
	sphere_material->set_emission_color(glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	sphere_material->set_specular_color(glm::vec3{ 0.332741f, 0.328634f, 0.346435f });
	sphere_material->set_specular_exponent(38.4f);
	sphere_material->set_diffuse_color(glm::vec4{ 0.18275f, 0.17f, 0.22525f, 1.0f });
	sphere_material->set_ambient_color(glm::vec3{ 0.05375f, 0.05f, 0.06625f});

	auto lamp_geometry_data = geometry_generators::generate_sphere_geometry_data(0.25f, 30, 30);
	auto lamp_geometry = std::make_shared<ES2Geometry>(lamp_geometry_data.first, lamp_geometry_data.second);
	auto lamp_material = std::make_shared<ES2ConstantMaterial>();
	lamp_material->set_emission_color(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	//OBJECTS

	auto plane = std::make_shared<Mesh>(plane_geometry, plane_material, glm::vec3{ 0.0f, -0.20f, 0.0f });
	plane->set_rotation_x(-1.57);
	plane->set_scale(glm::vec3{ 10.0f });

	auto sphere = std::make_shared<Mesh>(sphere_geometry, sphere_material, glm::vec3{ 0.0f, 0.0f, 0.0f });
	sphere->set_scale(glm::vec3{ 0.3f });

	auto lamp_1 = std::make_shared<Mesh>(lamp_geometry, lamp_material, glm::vec3{ 0.0f, 0.0f, 0.0f });
	lamp_1->set_scale(glm::vec3{ 0.1f });
	auto lamp_2 = std::make_shared<Mesh>(lamp_geometry, lamp_material, glm::vec3{ 0.0f, 0.0f, 0.0f });
	lamp_2->set_scale(glm::vec3{ 0.1f });

    std::vector<std::shared_ptr<Object>> objects { };
	objects.push_back(plane);
	objects.push_back(sphere);
	//objects.push_back(lamp_1);
	//objects.push_back(lamp_2);

	auto scene = std::make_shared<Scene>(objects);
	auto camera = scene->get_camera();
	auto root = scene->get_root();
	camera->set_z(1.5f);
	camera->set_y(camera->get_y() + 0.8f);
	camera->set_rotation_x(-0.5f);

	scene->get_directional_lights().clear();
	scene->get_point_lights().clear();
	scene->get_spot_lights().clear();
	auto point_light_1 = std::make_shared<PointLight>();
	point_light_1->set_intensity(0.36f);
	point_light_1->add_to_y(0.2f);
	point_light_1->add_child(lamp_1);
	auto point_light_2 = std::make_shared<PointLight>();
	point_light_2->set_intensity(0.45f);
	point_light_2->add_to_y(0.1f);
	point_light_2->add_child(lamp_2);
	root->add_child(point_light_1);
	root->add_child(point_light_2);
	scene->get_point_lights().push_back(point_light_1);
	scene->get_point_lights().push_back(point_light_2);

	window->set_on_key([&](int key) {
		switch (key) {
		case SDLK_w: camera->set_position(camera->get_position() - glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED)); break;
		case SDLK_a: camera->set_position(camera->get_position() - glm::vec3(camera->get_model_matrix() * SIDE * CAMERA_SPEED)); break;
		case SDLK_s: camera->set_position(camera->get_position() + glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED)); break;
		case SDLK_d: camera->set_position(camera->get_position() + glm::vec3(camera->get_model_matrix() * SIDE * CAMERA_SPEED)); break;
		case SDLK_e: camera->set_y(camera->get_y() + CAMERA_ROT_SPEED); break;
		case SDLK_q: camera->set_y(camera->get_y() - CAMERA_ROT_SPEED); break;
		case SDLK_UP: camera->set_position(camera->get_position() - glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED));   break;
		case SDLK_DOWN: camera->set_position(camera->get_position() + glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED)); break;
		case SDLK_LEFT: camera->set_position(camera->get_position() - glm::vec3(camera->get_model_matrix() * SIDE * CAMERA_SPEED));   break;
		case SDLK_RIGHT: camera->set_position(camera->get_position() + glm::vec3(camera->get_model_matrix() * SIDE * CAMERA_SPEED)); break;
		case SDLK_ESCAPE: exit(0); break;
		default: break;
		}
	});
	window->set_on_button([&](int x, int y, uint32_t type, uint8_t state) {
		if (type == SDL_MOUSEWHEEL)
		{
			camera->set_position(camera->get_position() - glm::vec3(camera->get_model_matrix() * glm::vec4{ 0.0f, 0.0f, y, 0.0f} * CAMERA_SPEED));
		}
		else if (type == SDL_MOUSEMOTION)
		{
			if(y < 0) camera->set_rotation_x(camera->get_rotation_x() + CAMERA_MOUSE_ROT_SPEED);
			else if(y > 0) camera->set_rotation_x(camera->get_rotation_x() - CAMERA_MOUSE_ROT_SPEED);

			if(x < 0) camera->set_rotation_y(camera->get_rotation_y() + CAMERA_MOUSE_ROT_SPEED);
			else if(x > 0) camera->set_rotation_y(camera->get_rotation_y() - CAMERA_MOUSE_ROT_SPEED);

			
		}
	});
    ES2Renderer renderer(scene, window);
	int i = 0;
	float  j = 0.0f;
	time_t now = time(0);
	tm* local_time = localtime(&now);
	int sec = 0;

    for (;;) {
        window->poll();

		now = time(0);
		local_time = localtime(&now);
		sec = local_time->tm_sec;

		float angle = (float)i / (float)200 * 2 * M_PI;
		float x = cosf(angle) * 0.5f;
		float z = sinf(angle) * 0.5f;
		scene->set_time((float) sinf(j));
        // TODO
		point_light_1->set_position(glm::vec3{x, point_light_1->get_y(), z });
		point_light_2->set_position(glm::vec3{ x, point_light_2->get_y(),-z });

		i = i % 200 + 1;
		j += 0.01f;
        renderer.render();
    }
}
