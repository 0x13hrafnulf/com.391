#pragma once
#include "aur.hpp"

class Agent
{
public:
	enum State
	{
		Alive,
		Reloading,
		Reload,
		Dying,
		Dead,
		Attack
	};

	Agent()
	{
	}

	~Agent()
	{};

	virtual void update(float delta_time) = 0;

	glm::vec3 get_position()
	{
		return _position;
	}
	const State get_state() const
	{
		return _state;
	}

	const std::shared_ptr<aur::Mesh> &get_mesh() const
	{
		return _mesh;
	}

	const std::shared_ptr<aur::Sphere>& get_bounding_volume()
	{
		return std::make_shared<aur::Sphere>(_bounding_volume);
	}
	bool intersects_with_ray(const aur::Ray& ray) const
	{
		return ray.intersects_with_sphere(_bounding_volume).first;
	}
protected:
	std::shared_ptr<aur::ES2Texture> _texture;
	unsigned int _texture_frame{ 0 };
	unsigned int _texture_frames{ 1 };
	
	std::shared_ptr<aur::Mesh> _mesh;

	State _state{ Alive };

	int _update_request{ 0 };
	int _update_rate{ 10 };

	aur::Sphere _bounding_volume{ glm::vec3{0.0f}, 1.0f };

	glm::vec3 _position{ 0.0f };
	float _size = 0.0f;
	void _set_texture_frame(unsigned int texture_frame)
	{
		_texture_frame = texture_frame;
		glm::mat4 texture_transformation_matrix = _texture->get_transformation_matrix();
		texture_transformation_matrix[3][0] = static_cast<float>(_texture_frame) / static_cast<float>(_texture_frames);
		_texture->set_transformation_matrix(texture_transformation_matrix);
	}
	void _set_texture_frames(unsigned int texture_frames)
	{
		_texture_frames = texture_frames;
		glm::mat4 texture_transformation_matrix = _texture->get_transformation_matrix();
		texture_transformation_matrix[0][0] = 1.0f / static_cast<float>(_texture_frames);
		texture_transformation_matrix[3][0] = static_cast<float>(_texture_frame) / static_cast<float>(_texture_frames);
		_texture->set_transformation_matrix(texture_transformation_matrix);
	}

};