#ifndef MESH_HPP
#define MESH_HPP

#include "object.hpp"

#include <memory>
#include <utility>
#include "es2_geometry.hpp"
#include "es2_constant_material.hpp"

class Mesh : public Object {
public:
	Mesh(std::shared_ptr<ES2Geometry> geometry, std::shared_ptr<ES2ConstantMaterial> material,
		const glm::vec3& position = glm::vec4(0.0f),
		const glm::vec3& rotation = glm::vec4(0.0f),
		const glm::vec3& scale = glm::vec4(1.0f)) :
		Object(position, rotation, scale)
	{
		_geometry = geometry;
		_material = material;
	}

    const std::shared_ptr<Geometry> &get_geometry() const {
        return _geometry;
    }

    const std::shared_ptr<Material> &get_material() const {
        return _material;
    }

private:
    std::shared_ptr<Geometry> _geometry;
    std::shared_ptr<Material> _material;
};

#endif
