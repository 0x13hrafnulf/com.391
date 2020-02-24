#include "aur.hpp"
#include <ctime>
static const float CAMERA_SPEED { 0.1f };
static const float CAMERA_ROT_SPEED { 0.05f };

static const glm::vec4 FORWARD { 0.0f, 0.0f, 1.0f, 0.0f };
static const float CLOCK_RADIUS{ 3.0f };
static const float HOUR_RADIUS{ 2.0f };
static const float MIN_RADIUS{ 2.7f };
static const float SEC_RADIUS{ 2.9f };
[[ noreturn ]]
int main(int argc, char **argv)
{
    using namespace aur;

    auto window = std::make_shared<SDLWindow>("aur", 0, 0);

    std::vector<std::shared_ptr<Object>> objects;
	//Sphere
	auto sphere_geometry_data = geometry_generators::generate_sphere_geometry_data(0.5f, 20, 20);
	auto sphere_geometry = std::make_shared<ES2Geometry>(sphere_geometry_data.first, sphere_geometry_data.second);
	sphere_geometry->set_type(GL_LINE_LOOP);
	auto sphere_material = std::make_shared<ES2ConstantMaterial>();
	sphere_material->set_emission_color(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
	
	//Circle
	auto circle_geometry_data = geometry_generators::generate_circle_geometry_data(0.2f, 20);
	auto circle_geometry = std::make_shared<ES2Geometry>(circle_geometry_data.first, circle_geometry_data.second);
	circle_geometry->set_type(GL_TRIANGLE_FAN);
	auto circle_material = std::make_shared<ES2ConstantMaterial>();
	circle_material->set_emission_color(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });


	//Rectangle
	std::vector<unsigned int> rect_indices;
	std::vector<Vertex> rect_vertices;
	rect_vertices.push_back(Vertex{
			glm::vec3 { -0.5f, 0.5f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 0.0f, 1.0f, 0.0f, 1.0f }
	});
	rect_vertices.push_back(Vertex{
			glm::vec3 { -0.5f, -0.5f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 0.0f, 0.0f, 0.0f, 1.0f }
	});
	rect_vertices.push_back(Vertex{
			glm::vec3 { 0.5f, -0.5f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 1.0f, 0.0f, 0.0f, 1.0f }
	});
	rect_vertices.push_back(Vertex{
			glm::vec3 { 0.5f, 0.5f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 1.0f, 1.0f, 0.0f, 1.0f }
	});
	rect_indices.push_back(0);
	rect_indices.push_back(1);
	rect_indices.push_back(2);

	rect_indices.push_back(2);
	rect_indices.push_back(3);
	rect_indices.push_back(0);

	auto rectangle_geometry_data = std::make_pair(rect_indices, rect_vertices);
	auto rectangle_geometry = std::make_shared<ES2Geometry>(rectangle_geometry_data.first, rectangle_geometry_data.second);
	rectangle_geometry->set_type(GL_TRIANGLE_FAN);
	auto rectangle_material = std::make_shared<ES2ConstantMaterial>();
	rectangle_material->set_emission_color(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
	auto hour_rectangle_material = std::make_shared<ES2ConstantMaterial>();
	hour_rectangle_material->set_emission_color(glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f });

	//OBJECTS
	auto center = std::make_shared<Mesh>(sphere_geometry, sphere_material, glm::vec3{0.0f, 0.0f, 0.0f});
	center->set_scale(glm::vec3{ 0.25f });
	
	//HANDS
	auto hour_hand = std::make_shared<Mesh>(rectangle_geometry, rectangle_material, glm::vec3{CLOCK_RADIUS / 2.0f - 0.5f, 0.0f, 0.0f });
	hour_hand->set_scale(glm::vec3{ 0.08f });
	hour_hand->set_scale_x(HOUR_RADIUS);
	

	auto min_hand = std::make_shared<Mesh>(rectangle_geometry, rectangle_material, glm::vec3{ CLOCK_RADIUS / 2.0f - 0.15f, 0.0f, 0.0f });
	min_hand->set_scale(glm::vec3{ 0.08f });
	min_hand->set_scale_x(MIN_RADIUS);
	min_hand->set_scale_y(0.03f);
	

	auto sec_hand = std::make_shared<Mesh>(rectangle_geometry, rectangle_material, glm::vec3{ CLOCK_RADIUS / 2.0f, 0.0f, 0.0f });
	sec_hand->set_scale(glm::vec3{ 0.08f });
	sec_hand->set_scale_x(SEC_RADIUS);
	sec_hand->set_scale_y(0.01f);
	
	//HOURS AND QUATERS
	//Rectangle
	std::vector<unsigned int> rhomb_indices;
	std::vector<Vertex> rhomb_vertices;
	rhomb_vertices.push_back(Vertex{
			glm::vec3 { 0.0f, 0.5f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 0.0f, 1.0f, 0.0f, 1.0f }
		});
	rhomb_vertices.push_back(Vertex{
			glm::vec3 { -0.5f, 0.0f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 0.0f, 0.0f, 0.0f, 1.0f }
		});
	rhomb_vertices.push_back(Vertex{
			glm::vec3 { 0.0f, -0.5f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 1.0f, 0.0f, 0.0f, 1.0f }
		});
	rhomb_vertices.push_back(Vertex{
			glm::vec3 { 0.5f, 0.0f, 0.0f },
			glm::vec4 { 1.0f },
			glm::vec3 { 0.0f, 0.0f, 1.0f },
			glm::vec4 { 1.0f, 1.0f, 0.0f, 1.0f }
		});
	rhomb_indices.push_back(0);
	rhomb_indices.push_back(1);
	rhomb_indices.push_back(2);

	rhomb_indices.push_back(2);
	rhomb_indices.push_back(3);
	rhomb_indices.push_back(0);

	auto rhomb_geometry_data = std::make_pair(rhomb_indices, rhomb_vertices);
	auto rhomb_geometry = std::make_shared<ES2Geometry>(rhomb_geometry_data.first, rhomb_geometry_data.second);
	rhomb_geometry->set_type(GL_TRIANGLE_FAN);
	auto rhomb_material = std::make_shared<ES2ConstantMaterial>();
	rhomb_material->set_emission_color(glm::vec4{ 0.5f, 0.5f, 0.5f, 1.0f });



	for (int i = 0; i < 12; ++i)
	{
		float angle = (float) i / (float) 12 * 2 * M_PI;
		float x = cosf(angle) * (CLOCK_RADIUS + 0.25f);
		float y = sinf(angle) * (CLOCK_RADIUS + 0.25f);
		if (i % 3 == 0)
		{
			auto quat = std::make_shared<Mesh>(rhomb_geometry, rectangle_material, glm::vec3{x, y, -0.05f });
			quat->set_scale(glm::vec3{ 0.4f });
			objects.push_back(quat);
		}
		auto rhomb = std::make_shared<Mesh>(rhomb_geometry, rhomb_material, glm::vec3{x, y, -0.15f });
		rhomb->set_scale(glm::vec3{ 0.2f });
		objects.push_back(rhomb);
	}

	//CIRCLES
	for (int i = 0; i < 60; ++i)
	{
		float angle = (float)i / (float)60 * 2 * M_PI;
		float x = cosf(angle) * CLOCK_RADIUS;
		float y = sinf(angle) * CLOCK_RADIUS;
		auto circle = std::make_shared<Mesh>(circle_geometry, circle_material, glm::vec3{ x, y, 0.0f });
		circle->set_scale(glm::vec3{ 0.2f });
		objects.push_back(circle);
	}

	//
	objects.push_back(center);
	objects.push_back(hour_hand);
	objects.push_back(min_hand);
	objects.push_back(sec_hand);

    // TODO
    auto scene = std::make_shared<Scene>(objects);
    auto root = scene->get_root();

	

    auto &camera = scene->get_camera();
    camera.set_z(7.0f);

    window->set_on_key([&](int key) {
        switch (key) {
            case SDLK_w: camera.set_rotation_x(camera.get_rotation_x() - CAMERA_ROT_SPEED); break;
            case SDLK_a: camera.set_rotation_y(camera.get_rotation_y() + CAMERA_ROT_SPEED); break;
            case SDLK_s: camera.set_rotation_x(camera.get_rotation_x() + CAMERA_ROT_SPEED); break;
            case SDLK_d: camera.set_rotation_y(camera.get_rotation_y() - CAMERA_ROT_SPEED); break;
            case SDLK_e: camera.set_y(camera.get_y() + CAMERA_ROT_SPEED); break;
            case SDLK_q: camera.set_y(camera.get_y() - CAMERA_ROT_SPEED); break;
            case SDLK_UP: camera.set_position(camera.get_position() - glm::vec3(camera.get_model_matrix() * FORWARD * CAMERA_SPEED));   break;
            case SDLK_DOWN: camera.set_position(camera.get_position() + glm::vec3(camera.get_model_matrix() * FORWARD * CAMERA_SPEED)); break;
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
		int hour = local_time->tm_hour;
		int min = local_time->tm_min;
		int sec = local_time->tm_sec;

		float local_hour = (float) hour / (float) 12 * 2 * (float) M_PI;
		float local_min = (float) min / (float) 60 * 2 * (float) M_PI;
		float local_sec = (float) sec / (float) 60 * 2 * (float) M_PI;

		hour_hand->set_position(glm::vec3 {cosf(local_hour), sinf(local_hour), 0.0f});
		min_hand->set_position(glm::vec3{ cosf(local_min) * MIN_RADIUS / 2.0f, sinf(local_min) * MIN_RADIUS / 2.0f, 0.0f });
		sec_hand->set_position(glm::vec3{ cosf(local_sec) * SEC_RADIUS / 2.0f, sinf(local_sec) * SEC_RADIUS / 2.0f, 0.0f });

		hour_hand->set_rotation_z(local_hour);
		min_hand->set_rotation_z(local_min);
		sec_hand->set_rotation_z(local_sec);
		root->set_rotation_y(root->get_rotation_y() + 0.005f);
        renderer.render();
    }
}
