#include "aur.hpp"

static const float CAMERA_SPEED { 0.01f };
static const float CAMERA_ROT_SPEED { 0.01f };
static const float TRIANGLE_ROT_SPEED { 0.01f };

static const glm::vec4 FORWARD { 0.0f, 0.0f, 1.0f, 0.0f };

int main(int argc, char **argv)
{
    using namespace aur;

    auto window = std::make_shared<SDLWindow>("aur", 500, 500);




	std::vector<Vertex> circleVertices;

	for (int i = 0; i < 40; ++i)
	{
		float angle = (float)i / (float)20 * 2 * M_PI;
		float x = cosf(angle) * 0.5f;
		float y = sinf(angle) * 0.5f;
		circleVertices.push_back(Vertex{ {x, y, 0.0f} });
	}
	auto circleGeometry = std::make_shared<ES2Geometry>(circleVertices);
	circleGeometry->set_type(GL_TRIANGLE_FAN);
	auto circleMaterial = std::make_shared<ES2ConstantMaterial>();
	circleMaterial->set_emission_color(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

    std::vector<Vertex> triangleVertices {
        Vertex { {-0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        Vertex { { 0.0f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        Vertex { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };
    auto triangleGeometry = std::make_shared<ES2Geometry>(triangleVertices);
    std::vector<Vertex> rectangleVertices {
        Vertex { {-0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        Vertex { {-0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        Vertex { { 0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        Vertex { { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }
    };


    auto rectangleGeometry = std::make_shared<ES2Geometry>(rectangleVertices);
    rectangleGeometry->set_type(GL_TRIANGLE_FAN);
    auto material = std::make_shared<ES2ConstantMaterial>();

    auto triangle = std::make_shared<Mesh>(triangleGeometry, material, glm::vec3 { -1.0f, 0.0f, 0.0f });
    triangle->set_name("triangle");

    auto rectangle = std::make_shared<Mesh>(rectangleGeometry, material, glm::vec3 { 1.0f, 0.0f, 0.0f });
    rectangle->set_name("rectangle");

	auto circle = std::make_shared<Mesh>(circleGeometry, circleMaterial, glm::vec3{ 0.0f, 0.0f, 0.0f });
	circle->set_name("circle");

	circle->add_child(triangle);
	triangle->add_child(rectangle);


    std::vector<std::shared_ptr<Object>> objects { circle };
    auto scene = std::make_shared<Scene>(objects);

    auto &camera = scene->get_camera();
    camera.set_z(5.0f);

    ES2Renderer renderer(scene, window);

    for (;;) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto end;
            } else {
                switch (event.key.keysym.sym) {
                    case SDLK_w: {
                        camera.set_rotation_x(camera.get_rotation_x() - CAMERA_ROT_SPEED);
                        break;
                    }
                    case SDLK_a: {
                        camera.set_rotation_y(camera.get_rotation_y() + CAMERA_ROT_SPEED);
                        break;
                    }
                    case SDLK_s: {
                        camera.set_rotation_x(camera.get_rotation_x() + CAMERA_ROT_SPEED);
                        break;
                    }
                    case SDLK_d: {
                        camera.set_rotation_y(camera.get_rotation_y() - CAMERA_ROT_SPEED);
                        break;
                    }
                    case SDLK_e: {
                        camera.set_y(camera.get_y() + CAMERA_ROT_SPEED);
                        break;
                    }
                    case SDLK_q: {
                        camera.set_y(camera.get_y() - CAMERA_ROT_SPEED);
                        break;
                    }
                    case SDLK_UP: {
                        camera.set_position(camera.get_position() - glm::vec3(camera.get_model_matrix() * FORWARD * CAMERA_SPEED));
                        break;
                    }
                    case SDLK_DOWN: {
                        camera.set_position(camera.get_position() + glm::vec3(camera.get_model_matrix() * FORWARD * CAMERA_SPEED));
                        break;
                    }
                }
            }
        }

        circle->set_rotation_y(circle->get_rotation_y() + TRIANGLE_ROT_SPEED);
        triangle->set_rotation_x(triangle->get_rotation_x() + TRIANGLE_ROT_SPEED);
		triangle->set_rotation_y(triangle->get_rotation_y() + TRIANGLE_ROT_SPEED);

        renderer.render();
    }

end:
    return 0;
}
