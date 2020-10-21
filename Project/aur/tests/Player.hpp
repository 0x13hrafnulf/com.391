#pragma once
#include "Agent.hpp"
#include "Decoration.hpp"
class Player : public Agent
{
public:
	Player(const glm::vec3& position, float size, const std::string& sprite_file, unsigned int frame_count, unsigned int reload_frame)
	{
		
		auto [image_data, image_width, image_height, image_channels] = aur::file_utilities::read_image_file(sprite_file);
		_texture = std::make_shared<aur::ES2Texture>(image_data, image_width, image_height, image_channels);
		_texture->set_minification_filter(aur::Texture::FilterType::Nearest);
		_texture->set_magnification_filter(aur::Texture::FilterType::Nearest);
		_texture->set_mode(aur::Texture::Mode::Modulation);
		_texture->set_transformation_enabled(true);
		_set_texture_frames(frame_count);
		_set_reload_texture_frame(reload_frame);


		auto [indices, vertices] = aur::geometry_generators::generate_plane_geometry_data(size, size, 1, 1);
		auto geometry = std::make_shared<aur::ES2Geometry>(indices, vertices);
		auto material = std::make_shared<aur::ES2ConstantMaterial>();
		material->set_texture_1(_texture);
		material->set_blending_enabled(true);
		material->set_overlay(true);
		_position = position;
		_mesh = std::make_shared<aur::Mesh>(geometry, material);
		_mesh->set_position(position);
		_bounding_volume = aur::Sphere{_mesh->get_position(), size / 2.0f };
	}
	~Player()
	{
	}
	void update(float delta_time) override
	{
		_position = _camera->get_position();
		if (_state == Reloading)
		{
			if (_update_request++ % _update_rate != 0)
			{
				return;
			}
			unsigned int frame = _texture_frame + 1;
			if (frame >= _texture_frames)
			{
				_state = Reload;
			}
			else
			{
				_set_texture_frame(frame);
			}
		}
		else if (_state == Reload)
		{
			if (_update_request++ % _update_rate != 0)
			{
				return;
			}
			_set_texture_frame((_texture_frame + 1) % _reload_texture_frame);

			if (_state == Reload) _state = Alive;
		}
		else if (_state == Alive)
		{
			if (_update_request++ % _update_rate != 0 )
			{
				return;
			}

			_set_texture_frame((_texture_frame + 1) % _reload_texture_frame);
		}
	}

	void shoot()
	{
		_state = Reloading;
	}
	void kill()
	{
		if (_state == Alive)
		{
			_state = Dying;
			_camera->set_zoom(-2.0f);
			
			//_set_texture_frame(_first_dying_texture_frame);
		}
	}

	const std::shared_ptr<aur::Camera> get_camera() const
	{
		return _camera;
	}

	void set_camera( std::shared_ptr<aur::Camera>& camera)
	{
		_camera = camera;
	}

	bool check_position(const std::shared_ptr<Decoration>& decoration)
	{
		glm::vec3 decoration_position = decoration->get_position();
		glm::vec2 decoration_size = decoration->get_size();
		float minX = decoration_position.x - decoration_size.x/2;
		float maxX = decoration_position.x + decoration_size.x/2;
		float minZ = decoration_position.z - decoration_size.y/2;
		float maxZ = decoration_position.z + decoration_size.y/2;

		return (_position.x >= minX && _position.x <= maxX) && (_position.z >= minZ && _position.z <= maxZ);
	}

private:
	unsigned int _reload_texture_frame{ 0 };
	std::shared_ptr<aur::Camera> _camera{ nullptr };
	void _set_reload_texture_frame(unsigned int reload_texture_frame)
	{
		_reload_texture_frame = reload_texture_frame;
	}
};