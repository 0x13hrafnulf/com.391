#include "aur.hpp"
static const float CAMERA_SPEED{ 0.1f };
static const float CAMERA_ROT_SPEED{ 0.05f };

static const glm::vec4 FORWARD{ 0.0f, 0.0f, 1.0f, 0.0f };


int main(int argc, char **argv)
{
    using namespace aur;

    auto window = std::make_shared<SDLWindow>("aur", 0, 0);


    // TODO

	//RECTANGLES
	auto rect_geometry_data_1 = geometry_generators::generate_plane_geometry_data(1.0f, 1.0f, 1, 1);
	auto rect_geometry_1 = std::make_shared<ES2Geometry>(rect_geometry_data_1.first, rect_geometry_data_1.second);
	rect_geometry_1->set_type(Geometry::Type::TriangleFan);
	auto rect_material_1 = std::make_shared<ES2ConstantMaterial>();
	rect_material_1->set_emission_color(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	auto rect_geometry_data_2 = geometry_generators::generate_plane_geometry_data(1.0f, 1.0f, 15, 15);
	auto rect_geometry_2 = std::make_shared<ES2Geometry>(rect_geometry_data_2.first, rect_geometry_data_2.second);
	rect_geometry_2->set_type(Geometry::Type::LineLoop);
	auto rect_material_2 = std::make_shared<ES2ConstantMaterial>();
	rect_material_2->set_emission_color(glm::vec4{ 0.0f, 1.0f, 0.0f, 0.0f });

	auto rect_geometry_data_3 = geometry_generators::generate_plane_geometry_data(1.0f, 1.0f, 20, 20);
	auto rect_geometry_3 = std::make_shared<ES2Geometry>(rect_geometry_data_3.first, rect_geometry_data_3.second);
	rect_geometry_3->set_type(Geometry::Type::Points);
	auto rect_material_3 = std::make_shared<ES2ConstantMaterial>();
	rect_material_3->set_emission_color(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });


	//CIRCLES
	auto circle_geometry_data_1 = geometry_generators::generate_circle_geometry_data(0.2f, 10);
	auto circle_geometry_1 = std::make_shared<ES2Geometry>(circle_geometry_data_1.first, circle_geometry_data_1.second);
	circle_geometry_1->set_type(Geometry::Type::TriangleFan);
	auto circle_material_1 = std::make_shared<ES2ConstantMaterial>();
	circle_material_1->set_emission_color(glm::vec4{ 0.8f, 0.0f, 0.8f, 1.0f });

	auto circle_geometry_data_2 = geometry_generators::generate_circle_geometry_data(0.2f, 50);
	auto circle_geometry_2 = std::make_shared<ES2Geometry>(circle_geometry_data_2.first, circle_geometry_data_2.second);
	circle_geometry_2->set_type(Geometry::Type::LineLoop);
	auto circle_material_2 = std::make_shared<ES2ConstantMaterial>();
	circle_material_2->set_emission_color(glm::vec4{ 0.8f, 0.8f, 0.0f, 1.0f });

	auto circle_geometry_data_3 = geometry_generators::generate_circle_geometry_data(0.2f, 30);
	auto circle_geometry_3 = std::make_shared<ES2Geometry>(circle_geometry_data_3.first, circle_geometry_data_3.second);
	circle_geometry_3->set_type(Geometry::Type::Points);
	auto circle_material_3 = std::make_shared<ES2ConstantMaterial>();
	circle_material_3->set_emission_color(glm::vec4{ 0.5f, 0.5f, 0.5f, 1.0f });
	

	//SPHERES
	auto sphere_geometry_data_1 = geometry_generators::generate_sphere_geometry_data(0.25f, 10, 10);
	auto sphere_geometry_1 = std::make_shared<ES2Geometry>(sphere_geometry_data_1.first, sphere_geometry_data_1.second);
	sphere_geometry_1->set_type(Geometry::Type::Triangles);
	auto sphere_material_1 = std::make_shared<ES2ConstantMaterial>();
	sphere_material_1->set_emission_color(glm::vec4{ 0.7f, 0.5f, 0.0f, 1.0f });

	auto sphere_geometry_data_2 = geometry_generators::generate_sphere_geometry_data(0.25f, 20, 20);
	auto sphere_geometry_2 = std::make_shared<ES2Geometry>(sphere_geometry_data_2.first, sphere_geometry_data_2.second);
	sphere_geometry_2->set_type(Geometry::Type::LineLoop);
	auto sphere_material_2 = std::make_shared<ES2ConstantMaterial>();
	sphere_material_2->set_emission_color(glm::vec4{ 1.0f, 0.0f, 0.5f, 1.0f });

	auto sphere_geometry_data_3 = geometry_generators::generate_sphere_geometry_data(0.25f, 20, 20);
	auto sphere_geometry_3 = std::make_shared<ES2Geometry>(sphere_geometry_data_3.first, sphere_geometry_data_3.second);
	sphere_geometry_3->set_type(Geometry::Type::Points);
	auto sphere_material_3 = std::make_shared<ES2ConstantMaterial>();
	sphere_material_3->set_emission_color(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	//Objects

	auto rect_1 = std::make_shared<Mesh>(rect_geometry_1, rect_material_1, glm::vec3{ -0.5f, 0.5f, 0.0f });
	rect_1->set_scale(glm::vec3{ 0.3f });
	auto rect_2 = std::make_shared<Mesh>(rect_geometry_2, rect_material_2, glm::vec3{ 0.0f, 0.5f, 0.0f });
	rect_2->set_scale(glm::vec3{ 0.3f });
	auto rect_3 = std::make_shared<Mesh>(rect_geometry_3, rect_material_3, glm::vec3{ 0.5f, 0.5f, 0.0f });
	rect_3->set_scale(glm::vec3{ 0.3f });

	auto circle_1 = std::make_shared<Mesh>(circle_geometry_1, circle_material_1, glm::vec3{ -0.5f, 0.0f, 0.0f });
	circle_1->set_scale(glm::vec3{ 0.7f });
	auto circle_2 = std::make_shared<Mesh>(circle_geometry_2, circle_material_2, glm::vec3{ 0.0f, 0.0f, 0.0f });
	circle_2->set_scale(glm::vec3{ 0.7f });
	auto circle_3 = std::make_shared<Mesh>(circle_geometry_3, circle_material_3, glm::vec3{ 0.5f, 0.0f, 0.0f });
	circle_3->set_scale(glm::vec3{ 0.7f });


	auto sphere_1 = std::make_shared<Mesh>(sphere_geometry_1, sphere_material_1, glm::vec3{ -0.5f, -0.5f, 0.0f });
	sphere_1->set_scale(glm::vec3{ 0.7f });
	auto sphere_2 = std::make_shared<Mesh>(sphere_geometry_2, sphere_material_2, glm::vec3{ 0.0f, -0.5f, 0.0f });
	sphere_2->set_scale(glm::vec3{ 0.7f });
	auto sphere_3 = std::make_shared<Mesh>(sphere_geometry_3, sphere_material_3, glm::vec3{ 0.5f, -0.5f, 0.0f });
	sphere_3->set_scale(glm::vec3{ 0.7f });








    std::vector<std::shared_ptr<Object>> objects { };

	objects.push_back(rect_1);
	objects.push_back(rect_2);
	objects.push_back(rect_3);

	objects.push_back(circle_1);
	objects.push_back(circle_2);
	objects.push_back(circle_3);

	objects.push_back(sphere_1);
	objects.push_back(sphere_2);
	objects.push_back(sphere_3);


	auto scene = std::make_shared<Scene>(objects);
	auto camera = scene->get_camera();
	auto root = scene->get_root();
	camera->set_z(1.5f);
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
		rect_1->set_rotation_x(rect_1->get_rotation_x() + 0.01f);
		//rect_1->set_rotation_y(rect_1->get_rotation_y() + 0.01f);
		rect_1->set_rotation_z(rect_1->get_rotation_z() + 0.01f);
		circle_1->set_rotation_x(circle_1->get_rotation_x() + 0.01f);
		//circle_1->set_rotation_y(circle_1->get_rotation_y() + 0.01f);
		circle_1->set_rotation_z(circle_1->get_rotation_y() + 0.01f);
		sphere_1->set_rotation_x(sphere_1->get_rotation_z() + 0.01f);
		//sphere_1->set_rotation_y(sphere_1->get_rotation_y() + 0.01f);
		sphere_1->set_rotation_z(sphere_1->get_rotation_z() + 0.01f);

		rect_2->set_rotation_x(rect_2->get_rotation_x() + 0.01f);
		rect_2->set_rotation_y(rect_2->get_rotation_y() + 0.01f);
		//rect_2->set_rotation_z(rect_2->get_rotation_z() + 0.01f);
		circle_2->set_rotation_x(circle_2->get_rotation_x() + 0.01f);
		circle_2->set_rotation_y(circle_2->get_rotation_y() + 0.01f);
		//circle_2->set_rotation_z(circle_2->get_rotation_y() + 0.01f);
		sphere_2->set_rotation_x(sphere_2->get_rotation_z() + 0.01f);
		sphere_2->set_rotation_y(sphere_2->get_rotation_y() + 0.01f);
		//sphere_2->set_rotation_z(sphere_2->get_rotation_z() + 0.01f);

		//rect_3->set_rotation_x(rect_3->get_rotation_x() + 0.01f);
		rect_3->set_rotation_y(rect_3->get_rotation_y() + 0.01f);
		rect_3->set_rotation_z(rect_3->get_rotation_z() + 0.01f);
		//circle_3->set_rotation_x(circle_3->get_rotation_x() + 0.01f);
		circle_3->set_rotation_y(circle_3->get_rotation_y() + 0.01f);
		circle_3->set_rotation_z(circle_3->get_rotation_y() + 0.01f);
		//sphere_3->set_rotation_x(sphere_3->get_rotation_z() + 0.01f);
		sphere_3->set_rotation_y(sphere_3->get_rotation_y() + 0.01f);
		sphere_3->set_rotation_z(sphere_3->get_rotation_z() + 0.01f);


        renderer.render();
    }
}
