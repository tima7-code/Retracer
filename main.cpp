#include <random>
#include "Objects.h"
#include "Interface.h"

void to_shader(std::vector<object*> objects);

std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<> dist(0.0f, 1.0f);

int frames = 1;

int main()
{
	interface.setPosition(Vector2i(0, 0));
	window.setPosition(Vector2i(i_width, 0));

	interface_manager InterfaceManager;
	InterfaceManager.getIcons();

	std::vector<object*> objects = { &sphere(Vector3f( 100,   50,    0), Vector3f(255, 255, 255), 20, Vector3f(0.0, 1.5, 0.0)),
									 &sphere(Vector3f( 100,   40,   80), Vector3f(255, 255, 255), 20, Vector3f(0.5, 0.0, 0.0)),
									 &sphere(Vector3f( 150,   25,   90), Vector3f(255,   0,   0), 20, Vector3f(0.0, 0.0, 0.0)),
									 &cube  (Vector3f(-150,   50,    0), Vector3f(255, 255, 255), 30, Vector3f(1.0, 0.0, 0.0)),
									 &cube  (Vector3f(-330,   50,    0), Vector3f(255, 255, 255), 30, Vector3f(1.0, 0.0, 0.0)),
									 &cube  (Vector3f(   0,   50,   90), Vector3f(255, 255, 125), 30, Vector3f(0.0, 0.0, 0.0)),
									 &plane (Vector3f(   0,   20,    0), Vector3f(125, 125, 125),     Vector3f(0.0, 0.0, 0.0)),
									 &light (Vector3f( 400,  300,  200), Vector3f(255, 255, 255), 99                         ),
									 //&light (Vector3f(  30,   60,  -30), Vector3f(255, 255, 255), 20                         ),
									 &camera(Vector3f(   0,   50,  200)                                                      ) };

	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	RenderTexture texture1;
	texture1.create(width, height);
	Sprite sprite1      = Sprite(texture1.getTexture());
	Sprite copy_sprite1 = Sprite(texture1.getTexture());
	copy_sprite1.setScale(1, -1);
	copy_sprite1.setPosition(0, height);

	RenderTexture texture2;
	texture2.create(width, height);
	Sprite sprite2      = Sprite(texture2.getTexture());
	Sprite copy_sprite2 = Sprite(texture1.getTexture());
	copy_sprite2.setScale(1, -1);
	copy_sprite2.setPosition(0, height);

	shader.loadFromFile("shader.frag", Shader::Fragment);
	preview_shader.loadFromFile("preview_shader.frag", Shader::Fragment);

	int esc_counter = 1;
	bool select_button = false;

	while (window.isOpen() && interface.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			
			if (event.type == Event::KeyPressed)
			{
				if(event.key.code == Keyboard::Escape) esc_counter++;
				if(esc_counter % 2 == 0) window.setMouseCursorVisible(false);
				else                     window.setMouseCursorVisible(true);
			}
		}

		while (interface.pollEvent(event))
		{
			if (event.type == Event::Closed)
				interface.close();

			if (event.type == Event::MouseButtonReleased)
			{
				select_button = true;
				break;
			}

			if (event.type == Event::MouseButtonPressed && select_button)
			{
				InterfaceManager.ButtonPressed();
				select_button = false;
				break;
			}

			if (event.type == Event::MouseMoved && !select_button)
			{
				InterfaceManager.ButtonPressed();
				break;
			}
		}

		if (esc_counter % 2 == 0)
		{
			if (objects[objects.size() - 1]->rotation()) frames = 1;
			if (objects[objects.size() - 1]->move())     frames = 1;
		}

		if (brightness != bright_copy) frames = 1;
		bright_copy = brightness;

		if (sky_texture_changed) frames = 1;
		sky_texture_changed = false;

		to_shader(objects);

		if (frames % 2 == 1)
		{
			shader.setUniform("u_sample", texture1.getTexture());
			texture2.draw(copy_sprite1, &shader);
			window.draw(sprite2);
		}
		else
		{
			shader.setUniform("u_sample", texture2.getTexture());
			texture1.draw(copy_sprite2, &shader);
			window.draw(sprite1);
		}

		if (esc_counter % 2 != 0)
		{
			interface.clear(Color(83, 83, 83));
			InterfaceManager.DrawInterface();
		}

		window.display();
		interface.display();

		frames++;
	}

	return 0;
}

void to_shader(std::vector<object*> objects)
{
	const size_t object_number = 10;
	float    obj_name [object_number];
	float    obj_size [object_number];
	Vector3f obj_prop [object_number];
	Vector3f obj_coord[object_number];
	Vector3f obj_color[object_number];

	const size_t sources_number = 10;
	float    source_size [sources_number];
	Vector3f source_coord[sources_number];
	Vector3f source_color[sources_number];

	size_t array_size = objects.size() - 1;

	for (int i = 0; i < array_size; i++)
	{
		if (objects[i]->name == "sphere") { obj_name[i] = 1.0; }
		if (objects[i]->name == "cube")	  { obj_name[i] = 2.0; }
		if (objects[i]->name == "plane")  { obj_name[i] = 3.0; }

		if (objects[i]->name != "light")
		{
			obj_size[i]  = objects[i]->size;
			obj_prop[i]  = objects[i]->properties;
			obj_coord[i] = objects[i]->obj;
			obj_color[i] = objects[i]->color;
		}
		else if (objects[i]->name == "light")
		{
			source_size[i] = objects[i]->size;
			source_coord[i] = objects[i]->obj;
			source_color[i] = objects[i]->color;
		}
	}

	shader.setUniformArray("obj_coord", obj_coord, array_size);
	shader.setUniformArray("obj_color", obj_color, array_size);
	shader.setUniformArray("obj_name",  obj_name,  array_size);
	shader.setUniformArray("obj_size",  obj_size,  array_size);
	shader.setUniformArray("obj_prop",  obj_prop,  array_size);

	shader.setUniformArray("source_coord", source_coord, sources_number);
	shader.setUniformArray("source_color", source_color, sources_number);
	shader.setUniformArray("source_size",  source_size,  sources_number);

	shader.setUniform("cam", Vector3f(objects[array_size]->obj));
	shader.setUniform("u_resolution", Vector2f(width, height));

	shader.setUniform("seed1", Vector2f((float)dist(e2), (float)dist(e2)) * 999.0f);
	shader.setUniform("seed2", Vector2f((float)dist(e2), (float)dist(e2)) * 999.0f);

	shader.setUniform("u_sample_part", 1.0f / frames);
	shader.setUniform("u_skybox", sky_texture);
	shader.setUniform("brightness", brightness);
}