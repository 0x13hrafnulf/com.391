#ifndef ES2_GEOMETRY_HPP
#define ES2_GEOMETRY_HPP

#include "geometry.hpp"

#include <GL/glew.h>
#include <SDL.h>

#include <string>

class ES2Geometry : public Geometry {
public:
    explicit ES2Geometry(const std::vector<Vertex> &vertices) : Geometry(vertices) { }
	~ES2Geometry()
	{
		glDeleteVertexArrays(1, &_vertex_array_object);
		glDeleteBuffers(1, &_vertex_buffer_object);
	}
    void update(const Material &material) final {

		glGenBuffers(1, &_vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object);

		const auto* vertex_data = reinterpret_cast<const float*>(_vertices.data());
		const size_t size = _vertices.size() * sizeof(Vertex);

		glBufferData(GL_ARRAY_BUFFER, size, vertex_data, GL_STATIC_DRAW);

		bind();

        glGenVertexArrays(1, &_vertex_array_object);
        glBindVertexArray(_vertex_array_object);


        int position_attribute_location { material.get_shader().get_attributes().at("position") };
        if (position_attribute_location == -1) { return; }

        int color_attribute_location { material.get_shader().get_attributes().at("color") };
        if (color_attribute_location == -1) { return; }

        GLsizei stride { sizeof(GLfloat) * 7 };
        glEnableVertexAttribArray(position_attribute_location);
        glVertexAttribPointer(
            position_attribute_location,
            3, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) 0
        );
        glEnableVertexAttribArray(color_attribute_location);
        glVertexAttribPointer(
            color_attribute_location,
            4, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (sizeof(GLfloat) * 3)
        );

    }

    void bind() final {
        glBindVertexArray(_vertex_array_object);
    }
	void unbind() final
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
private:
	unsigned int _vertex_array_object;
	unsigned int _vertex_buffer_object;
};

#endif
