#pragma once
#include "aur.hpp"


class Decoration
{
public:
	enum Type
	{
		Floor,
		Wall,
		Door,
		Lava,
		Other
	};

	Decoration(const glm::vec3& position, const glm::vec3& rotation, float size_x, float size_y, Type type, float repeat_x, float repeat_y, const std::string& sprite_file, unsigned int frame_count)
	{
		if (type == Floor)
		{
			auto [indices, vertices] = aur::geometry_generators::generate_plane_geometry_data(size_x, size_y, 1, 1);
			auto geometry = std::make_shared<aur::ES2Geometry>(indices, vertices);
			auto material = std::make_shared<aur::ES2PhongMaterial>();
			auto [image_data, image_width, image_height, image_channels] = aur::file_utilities::read_image_file(sprite_file);
			auto _texture = std::make_shared<aur::ES2Texture>(image_data, image_width, image_height, image_channels);
			_texture->set_enabled(true);
			_texture->set_wrap_mode_s(aur::Texture::WrapMode::Repeat);
			_texture->set_wrap_mode_t(aur::Texture::WrapMode::Repeat);
			glm::mat4 texture_transformation_matrix{ 1.0f };
			texture_transformation_matrix[0][0] = 10.0f;
			texture_transformation_matrix[1][1] = 10.0f;
			_texture->set_transformation_enabled(true);
			_texture->set_transformation_matrix(texture_transformation_matrix);
			material->set_texture_1(_texture);

			material->set_specular_exponent(1.0f);
			material->set_specular_color(glm::vec3{ 0.0f });
			material->set_diffuse_color(glm::vec4{ 0.25f });
			_size = { size_x, size_y };
			_position = position;
			_rotation = rotation;
			_type = type;
			_mesh = std::make_shared<aur::Mesh>(geometry, material);
			_mesh->set_position(position);
			_mesh->set_rotation(rotation);
		}	
		else if (type == Wall) {
			auto [image_data, image_width, image_height, image_channels] = aur::file_utilities::read_image_file(sprite_file);
			_texture = std::make_shared<aur::ES2Texture>(image_data, image_width, image_height, image_channels);
			_texture->set_enabled(true);
			_texture->set_wrap_mode_s(aur::Texture::WrapMode::Repeat);
			_texture->set_wrap_mode_t(aur::Texture::WrapMode::Repeat);
			glm::mat4 texture_transformation_matrix{ 1.0f };
			texture_transformation_matrix[0][0] = repeat_x;
			texture_transformation_matrix[1][1] = repeat_y;
			_texture->set_transformation_enabled(true);
			_texture->set_transformation_matrix(texture_transformation_matrix);

			auto [indices, vertices] = aur::geometry_generators::generate_plane_geometry_data(50, 15, 1, 1);
			auto geometry = std::make_shared<aur::ES2Geometry>(indices, vertices);
			auto material = std::make_shared<aur::ES2PhongMaterial>();
			material->set_texture_1(_texture);
			material->set_specular_exponent(1.0f);
			material->set_specular_color(glm::vec3{ 0.0f });
			material->set_diffuse_color(glm::vec4{ 0.25f });
			_size = { size_x, size_y };
			_position = position;
			_rotation = rotation;
			_type = type;
			_mesh = std::make_shared<aur::Mesh>(geometry, material);
			_mesh->set_position(position);
			_mesh->set_rotation(rotation);
		}
	
	}
	~Decoration()
	{

	}

	void set_transofrmation_matrix(const glm::mat4 &texture_r_transformation_matrix)
	{
		_texture->set_transformation_matrix(texture_r_transformation_matrix);
	}
	void set_texture_mode(aur::Texture::Mode mode)
	{
		_texture->set_mode(mode);
	}
	void set_texture_wrapmode(aur::Texture::WrapMode wrap_mode_s, aur::Texture::WrapMode wrap_mode_t)
	{
		_texture->set_wrap_mode_s(wrap_mode_s);
		_texture->set_wrap_mode_t(wrap_mode_t);
	}
	void set_texture_filter_type(aur::Texture::FilterType min_filter, aur::Texture::FilterType max_filter)
	{
		_texture->set_minification_filter(min_filter);
		_texture->set_magnification_filter(max_filter);
	}

	void update(float delta_time)
	{
		if (_update_request++ % _update_rate != 0)
		{
			return;
		}
		unsigned int frame = _texture_frame + 1;
		_set_texture_frame(frame);	
	}

	glm::vec3 get_position()
	{
		return _position;
	}
	const Type get_state() const
	{
		return _type;
	}

	const std::shared_ptr<aur::Mesh>& get_mesh() const
	{
		return _mesh;
	}
	bool intersects_with_ray(const aur::Ray& ray) const
	{
		return ray.intersects_with_sphere(_bounding_volume).first;
	}

	const std::shared_ptr<aur::Sphere>& get_bounding_volume()
	{
		return std::make_shared<aur::Sphere>(_bounding_volume);
	}
	glm::vec2 get_size()
	{
		return _size;
	}

private:
	std::shared_ptr<aur::ES2Texture> _texture;
	unsigned int _texture_frame{ 0 };
	unsigned int _texture_frames{ 1 };

	std::shared_ptr<aur::Mesh> _mesh;

	Type _type;

	int _update_request{ 0 };
	int _update_rate{ 10 };

	aur::Sphere _bounding_volume{ glm::vec3{0.0f}, 1.0f };
	glm::vec2 _size{ 0.0f };
	glm::vec3 _position{ 0.0f };
	glm::vec3 _rotation{ 0.0f };

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