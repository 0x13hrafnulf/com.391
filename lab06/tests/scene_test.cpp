#include "aur.hpp"
#include <ctime>

static const float CAMERA_SPEED{ 0.1f };
static const float CAMERA_ROT_SPEED{ 0.05f };

static const glm::vec4 FORWARD{ 0.0f, 0.0f, 1.0f, 0.0f };
static const float CLOCK_RADIUS{ 3.0f };
static const float HOUR_RADIUS{ 2.0f };
static const float MIN_RADIUS{ 2.7f };
static const float SEC_RADIUS{ 2.9f };
int main(int argc, char **argv)
{
    using namespace aur;

    auto window = std::make_shared<SDLWindow>("aur", 0, 0);

	std::vector<std::shared_ptr<Object>> objects{ };
    // TODO
	auto sphere_geometry_data = geometry_generators::generate_sphere_geometry_data(0.25f, 20, 20);
	auto sphere_geometry = std::make_shared<ES2Geometry>(sphere_geometry_data.first, sphere_geometry_data.second);
	sphere_geometry->set_type(Geometry::Type::LineLoop);
	auto sphere_material = std::make_shared<ES2ConstantMaterial>();
	sphere_material->set_emission_color(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	auto circle_geometry_data = geometry_generators::generate_circle_geometry_data(0.2f, 20);
	auto circle_geometry = std::make_shared<ES2Geometry>(circle_geometry_data.first, circle_geometry_data.second);
	circle_geometry->set_type(Geometry::Type::TriangleFan);
	auto circle_material = std::make_shared<ES2ConstantMaterial>();
	circle_material->set_emission_color(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	auto rect_geometry_data = geometry_generators::generate_plane_geometry_data(1.0f ,1.0f , 1,1);
	auto rect_geometry = std::make_shared<ES2Geometry>(rect_geometry_data.first, rect_geometry_data.second);
	rect_geometry->set_type(Geometry::Type::TriangleFan);
	auto rect_material = std::make_shared<ES2ConstantMaterial>();
	rect_material->set_emission_color(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	auto rhomb_material = std::make_shared<ES2ConstantMaterial>();
	rhomb_material->set_emission_color(glm::vec4{ 0.5f, 0.5f, 0.5f, 1.0f });

	//OBJECTS
	auto center = std::make_shared<Mesh>(sphere_geometry, sphere_material, glm::vec3{ 0.0f, 0.0f, 0.0f });
	center->set_scale(glm::vec3{ 0.25f });

	auto hour_hand_center = std::make_shared<Object>("hour_hand", glm::vec3{ 0.0f, 0.0f, 0.0f });
	hour_hand_center->set_scale(glm::vec3{ 0.25f });
	auto hour_hand = std::make_shared<Mesh>(rect_geometry, rect_material, glm::vec3{ CLOCK_RADIUS / 2.0f - 0.5f, 0.0f, 0.0f });
	hour_hand->set_scale(glm::vec3{ 0.08f });
	hour_hand->set_scale_x(HOUR_RADIUS);
	hour_hand_center->add_child(hour_hand);

	auto min_hand_center = std::make_shared<Object>("min_hand", glm::vec3{ 0.0f, 0.0f, 0.0f });
	min_hand_center->set_scale(glm::vec3{ 0.25f });
	auto min_hand = std::make_shared<Mesh>(rect_geometry, rect_material, glm::vec3{ CLOCK_RADIUS / 2.0f - 0.15f, 0.0f, 0.0f });
	min_hand->set_scale(glm::vec3{ 0.08f });
	min_hand->set_scale_x(MIN_RADIUS);
	min_hand->set_scale_y(0.03f);
	min_hand_center->add_child(min_hand);


	auto sec_hand_center = std::make_shared<Object>("sec_hand", glm::vec3{ 0.0f, 0.0f, 0.0f });
	sec_hand_center->set_scale(glm::vec3{ 0.25f });
	auto sec_hand = std::make_shared<Mesh>(rect_geometry, rect_material, glm::vec3{ CLOCK_RADIUS / 2.0f, 0.0f, 0.0f });
	sec_hand->set_scale(glm::vec3{ 0.08f });
	sec_hand->set_scale_x(SEC_RADIUS);
	sec_hand->set_scale_y(0.01f);
	sec_hand_center->add_child(sec_hand);

	for (int i = 0; i < 12; ++i)
	{
		float angle = (float)i / (float)12 * 2 * M_PI;
		float x = cosf(angle) * (CLOCK_RADIUS + 0.25f);
		float y = sinf(angle) * (CLOCK_RADIUS + 0.25f);
		if (i % 3 == 0)
		{
			auto quat = std::make_shared<Mesh>(rect_geometry, rect_material, glm::vec3{ x, y, -0.05f });
			quat->set_scale(glm::vec3{ 0.4f });
			quat->set_rotation_z(0.8f);
			center->add_child(quat);
		}
		auto rhomb = std::make_shared<Mesh>(rect_geometry, rhomb_material, glm::vec3{ x, y, -0.15f });
		rhomb->set_scale(glm::vec3{ 0.2f });
		rhomb->set_rotation_z(0.8f);
		center->add_child(rhomb);
	}

	for (int i = 0; i < 60; ++i)
	{
		float angle = (float)i / (float)60 * 2 * M_PI;
		float x = cosf(angle) * CLOCK_RADIUS;
		float y = sinf(angle) * CLOCK_RADIUS;
		auto circle = std::make_shared<Mesh>(circle_geometry, circle_material, glm::vec3{ x, y, 0.0f });
		circle->set_scale(glm::vec3{ 0.2f });
		center->add_child(circle);
	}

	objects.push_back(center);
	objects.push_back(hour_hand_center);
	objects.push_back(min_hand_center);
	objects.push_back(sec_hand_center);


    auto scene = std::make_shared<Scene>(objects);
	auto camera = scene->get_camera();
	auto root = scene->get_root();
	camera->set_z(3.0f);
	window->set_on_key([&](int key) {
		switch (key) {
		case SDLK_w: camera->set_rotation_x(camera->get_rotation_x() - CAMERA_ROT_SPEED); break;
		case SDLK_a: camera->set_rotation_y(camera->get_rotation_y() + CAMERA_ROT_SPEED); break;
		case SDLK_s: camera->set_rotation_x(camera->get_rotation_x() + CAMERA_ROT_SPEED); break;
		case SDLK_d: camera->set_rotation_y(camera->get_rotation_y() - CAMERA_ROT_SPEED); break;
		case SDLK_e: camera->set_y(camera->get_y() + CAMERA_ROT_SPEED); break;
		case SDLK_q: camera->set_y(camera->get_y() - CAMERA_ROT_SPEED); break;
		case SDLK_UP: camera->set_position(camera->get_position() - glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED));   break;
		case SDLK_DOWN: camera->set_position(camera->get_position() + glm::vec3(camera->get_model_matrix() * FORWARD * CAMERA_SPEED)); break;
		case SDLK_ESCAPE: exit(0); break;
		default: break;
		}
	});

    ES2Renderer renderer(scene, window);

	for (;;) {
        window->poll();

        // TODO
		time_t now = time(0);
		tm* local_time = localtime(&now);
		int hour = 3 + local_time->tm_hour;
		int min = 15 + local_time->tm_min;
		int sec = 15 + local_time->tm_sec;

		float local_hour = (float)hour / (float)12 * 2 * (float)M_PI;
		float local_min = (float)min / (float)60 * 2 * (float)M_PI;
		float local_sec = (float)sec / (float)60 * 2 * (float)M_PI;

		//With no child
		//hour_hand->set_position(glm::vec3 {cosf(local_hour), sinf(local_hour), 0.0f});
		//min_hand->set_position(glm::vec3{ cosf(local_min) * MIN_RADIUS / 2.0f, sinf(local_min) * MIN_RADIUS / 2.0f, 0.0f });
		//sec_hand->set_position(glm::vec3{ cosf(local_sec) * SEC_RADIUS / 2.0f, sinf(local_sec) * SEC_RADIUS / 2.0f, 0.0f });

		hour_hand_center->set_rotation_z(local_hour);
		min_hand_center->set_rotation_z(local_min);
		sec_hand_center->set_rotation_z(local_sec);
		root->set_rotation_y(root->get_rotation_y() + 0.005f);
        renderer.render();
    }
}
