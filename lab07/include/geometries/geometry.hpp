#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "materials/material.hpp"
#include "geometries/vertex.hpp"

#include <vector>
#include <utility>

#include <glm/glm.hpp>

namespace aur
{
    class Geometry
    {
    public:
        enum Type
        {
            Points,
            Lines,
            LineLoop,
            LineStrip,
            Triangles,
            TriangleFan,
            TriangleStrip
        };

        enum UsageStrategy
        {
            StaticStrategy,
            DynamicStrategy,
            StreamStrategy
        };

        explicit Geometry(std::vector<unsigned int> indices, std::vector<Vertex> vertices)
            : _indices(std::move(indices)), _vertices(std::move(vertices))
        {
			compute_tangent_vectors();
		}

        virtual ~Geometry() = default;

        [[nodiscard]] Type get_type() const
        {
            return _type;
        }

        void set_type(Type type)
        {
            _type = type;
        }

        [[nodiscard]] const std::vector<unsigned int> &get_indices() const
        {
            return _indices;
        }

        [[nodiscard]] const std::vector<Vertex> &get_vertices() const
        {
            return _vertices;
        }

        [[nodiscard]] UsageStrategy get_vertices_usage_strategy() const
        {
            return _vertices_usage_strategy;
        }

        void set_vertices_usage_strategy(UsageStrategy vertices_usage_strategy)
        {
            if (_vertices_usage_strategy != vertices_usage_strategy) {
                _vertices_usage_strategy = vertices_usage_strategy;
                _requires_vertices_update = true;
            }
        }

        [[nodiscard]] UsageStrategy get_indices_usage_strategy() const
        {
            return _indices_usage_strategy;
        }

        void set_indices_usage_strategy(UsageStrategy indices_usage_strategy)
        {
            if (_indices_usage_strategy != indices_usage_strategy) {
                _indices_usage_strategy = indices_usage_strategy;
                _requires_indices_update = true;
            }
        }

        [[nodiscard]] float get_line_width() const
        {
            return _line_width;
        }

        void set_line_width(float lineWidth)
        {
            _line_width = lineWidth;
        }

        void transform(glm::mat4 transformation_matrix)
        {
            for (auto &vertex : _vertices) {
                vertex.position = glm::vec3(transformation_matrix * glm::vec4(vertex.position, 1.0f));
            }
            _requires_vertices_update = true;
        }

        virtual void update(const Material &material) = 0;

        virtual void use() = 0;

		void compute_tangent_vectors()
		{
			int vertex_count = _indices.size();
			std::vector<glm::vec3> tmp_Tangent(vertex_count);
			std::vector<glm::vec3> tmp_Binormal(vertex_count);

			for (int i = 0; i < vertex_count; ++i)
			{
				tmp_Tangent[i] = glm::vec3{0.0f};
				tmp_Binormal[i] = glm::vec3{0.0f};
			}

			for (int i = 0; i < vertex_count; i += 3)
			{
				Vertex v0 = _vertices[_indices[i]];
				Vertex v1 = _vertices[_indices[i+1]];
				Vertex v2 = _vertices[_indices[i+2]];

				glm::vec3 q1 = v1.position - v0.position;
				glm::vec3 q2 = v2.position - v0.position;
				float s1 = v1.texture1_coordinates.x - v0.texture1_coordinates.x;
				float s2 = v2.texture1_coordinates.x - v0.texture1_coordinates.x;
				float t1 = v1.texture1_coordinates.y - v0.texture1_coordinates.y;
				float t2 = v2.texture1_coordinates.y - v0.texture1_coordinates.y;

				glm::vec3 t_tangent = glm::normalize(t2 * q1 - t1 * q2);
				glm::vec3 t_binormal = glm::normalize(-s2 * q1 + s1 * q2);

	
				tmp_Tangent[_indices[i]] += t_tangent;
				tmp_Binormal[_indices[i]] += t_binormal;

				tmp_Tangent[_indices[i+1]] += t_tangent;
				tmp_Binormal[_indices[i+1]] += t_binormal;

				tmp_Tangent[_indices[i+2]] += t_tangent;
				tmp_Binormal[_indices[i+2]] += t_binormal;
			
			}

			for (int i = 0; i < vertex_count; ++i)
			{
				Vertex t_v = _vertices[_indices[i]];
				glm::vec3 t_tangent = tmp_Tangent[_indices[i]];

				t_tangent -= t_v.normal * glm::dot(t_tangent, t_v.normal);
				t_tangent = glm::normalize(t_tangent);
				_vertices[_indices[i]].tangent = glm::vec4(t_tangent, 0.0f);

				if (glm::dot(glm::cross(t_v.normal, t_tangent), tmp_Binormal[_indices[i]]) < 0.0f)
				{
					_vertices[_indices[i]].tangent.w = -1.0f;
				}
				else
				{
					_vertices[_indices[i]].tangent.w = 1.0f;
				}
			}
			//TODO
		}

    protected:
        Type _type{Triangles};

        std::vector<unsigned int> _indices;
        std::vector<Vertex> _vertices;
        bool _requires_indices_update{true};
        bool _requires_vertices_update{true};

        UsageStrategy _vertices_usage_strategy{StaticStrategy};
        UsageStrategy _indices_usage_strategy{StaticStrategy};

        float _line_width{1.0f};
    };
}

#endif
