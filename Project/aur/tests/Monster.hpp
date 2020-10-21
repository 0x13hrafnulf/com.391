#pragma once
#include "Agent.hpp"
#include "Player.hpp"
#include "math.h"

class Monster : public Agent
{
public:
	Monster(const glm::vec3 &position, float size, float speed, const std::string &sprite_file, unsigned int frame_count, unsigned int attack_frame, unsigned int first_dying_frame)
	{

		auto [image_data, image_width, image_height, image_channels] = aur::file_utilities::read_image_file(sprite_file);
		_texture = std::make_shared<aur::ES2Texture>(image_data, image_width, image_height, image_channels);
		_texture->set_minification_filter(aur::Texture::FilterType::Nearest);
		_texture->set_magnification_filter(aur::Texture::FilterType::Nearest);
		_texture->set_mode(aur::Texture::Mode::Modulation);
		_texture->set_transformation_enabled(true);
		_set_texture_frames(frame_count);
		_set_first_dying_texture_frame(first_dying_frame);
		_set_attack_frame(attack_frame);

		auto [indices, vertices] = aur::geometry_generators::generate_plane_geometry_data(size, size, 1, 1);
		auto geometry = std::make_shared<aur::ES2Geometry>(indices, vertices);
		auto material = std::make_shared<aur::ES2ConstantMaterial>();
		material->set_texture_1(_texture);
		material->set_face_culling_enabled(true);
		material->set_blending_enabled(true);
		material->set_transparent(true);

		_position = position;
		_speed = speed;
		_size = size;
		_mesh = std::make_shared<aur::Mesh>(geometry, material);
		_mesh->set_position(position);
		_bounding_volume = aur::Sphere{ _mesh->get_position(), size / 2.0f };
	}
	~Monster()
	{}
	

	void update(float delta_time) override
	{
		if (_state == Dying)
		{
			if (_update_request++ % _update_rate != 0)
			{
				return;
			}
			unsigned int frame = _texture_frame + 1;
			if (frame >= _texture_frames)
			{
				_state = Dead;
			}
			else
			{
				_set_texture_frame(frame);
			}
		}
		else if (_state == Alive)
		{
			if (_update_request++ % _update_rate != 0)
			{
				return;
			}
			_set_texture_frame((_texture_frame + 1) % _attack_frame);
			if(_state != Attack || _target->get_state() != Dead || _target->get_state() != Dying)
			{
				glm::vec3 target_position = _target->get_camera()->get_position();
				glm::vec3 chase_vector = target_position -_position;
				float distance = glm::length(chase_vector);
				if (distance != 0)
				{
					chase_vector /= distance;
				}
				if (distance <= _size / 2.0f)
				{
					_state = Attack;
					_set_texture_frame(_attack_frame);
					return;
				}
				glm::vec3 velocity = chase_vector * _speed;
				velocity.y = 0.0f;
				_mesh->add_to_position(velocity);
				_bounding_volume.set_center(_mesh->get_position());
				_position = _mesh->get_position();
			}
			
		}
		else if (_state == Attack)
		{
			if (_update_request++ % _update_rate != 0)
			{
				return;
			}
			unsigned int frame = _texture_frame + 1;
			if (frame >= _first_dying_texture_frame)
			{
				_set_texture_frame(_attack_frame);
				_target->kill();
			}
			else
			{
				_set_texture_frame(frame);
			}
			glm::vec3 target_position = _target->get_camera()->get_position();
			glm::vec3 chase_vector = target_position - _position;
			float distance = glm::length(chase_vector);
			if (distance >= _size / 2.0f)
			{
				_state = Alive;
				_set_texture_frame(0);
			}
		}

		_mesh->billboard_toward_camera(_target->get_camera());
	}


	void set_target(const std::shared_ptr<Player> &target)
	{
		_target = target;
	}
	void kill()
	{
		if (_state == Alive)
		{
			_state = Dying;
			_set_texture_frame(_first_dying_texture_frame);
		}
		
	}
	

	bool collision_monster(const std::shared_ptr<Monster>& monster) const
	{
		glm::vec3 pos = monster->get_position();
		float distance = sqrt((_position.x - pos.x) * (_position.x - pos.x) 
						+ (_position.y - pos.y) * (_position.y - pos.y) 
						+ (_position.z - pos.z) * (_position.z - pos.z));
		return distance < (_bounding_volume.get_radius() + monster->get_bounding_volume()->get_radius());
	}
	bool collision_Player(const std::shared_ptr<Player>& player) const
	{

	}
	bool collision_decoration(const std::shared_ptr<Monster>& monster) const
	{

	}
private:
	unsigned int _first_dying_texture_frame{ 0 };
	unsigned int _attack_frame{ 0 };
	std::shared_ptr<Player> _target{ nullptr };
	float _speed{ 1.0f };
	glm::vec3 _velocity{ 0.0f };


	void _set_first_dying_texture_frame(unsigned int first_dying_texture_frame)
	{
		_first_dying_texture_frame = first_dying_texture_frame;
	}
	void _set_attack_frame(unsigned int attack_frame)
	{
		_attack_frame = attack_frame;
	}
};