#include "Objects.h"

void to_shader(std::vector<object*> objects);

int main()
{
	std::vector<object*> objects = { &sphere(Vector3f( 100,  -50,    0), Vector3f(255,  25,  75), 20, 0.8),
									 &sphere(Vector3f( 100,  -50,   80), Vector3f( 75,  75, 255), 20, 0.8),
									 &cube  (Vector3f(-150,  -50,    0), Vector3f(255, 255, 255), 30, 0.9),
									 &cube  (Vector3f(-330,  -50,    0), Vector3f(255, 255, 255), 30, 0.9),
									 &cube  (Vector3f(   0,  -50,   90), Vector3f(255, 255, 125), 30, 0.0),
		                             &plane (Vector3f(   0,    1,    0), Vector3f(125, 125, 125),     1.0),
						             &camera(Vector3f(   0,    0,  200)                                  ) };

	window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);
	RenderTexture texture;
	texture.create(width, height);
	Sprite sprite = Sprite(texture.getTexture());
	shader.loadFromFile("shader.frag", Shader::Fragment);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		objects[objects.size() - 1]->rotation();
		objects[objects.size() - 1]->move();
		to_shader(objects);

		window.draw(sprite, &shader);
		window.display();
	}

	return 0;
}

void to_shader(std::vector<object*> objects)
{
	const size_t object_number = 10;
	float obj_name[object_number];
	float obj_size[object_number];
	float obj_ref [object_number];
	Vector3f obj_coord[object_number];
	Vector3f obj_color[object_number];

	int sources_number = 0;
	float source_size[object_number];
	Vector3f source_coord[object_number];
	Vector3f source_color[object_number];

	size_t array_size = objects.size() - 1;

	for (int i = 0; i < array_size; i++)
	{
		if (objects[i]->name == "sphere") { obj_name[i] = 1.0; }
		if (objects[i]->name == "cube") { obj_name[i] = 2.0; }
		if (objects[i]->name == "plane") { obj_name[i] = 3.0; }

		if (objects[i]->name != "light")
		{
			obj_size[i]  = objects[i]->size;
			obj_ref[i]   = objects[i]->reflectivity;
			obj_coord[i] = objects[i]->obj;
			obj_color[i] = objects[i]->color;
		}
		else if (objects[i]->name == "light")
		{
			sources_number++;
			source_size[i] = objects[i]->size;
			source_coord[i] = objects[i]->obj;
			source_color[i] = objects[i]->color;
		}
	}

	shader.setUniformArray("obj_coord", obj_coord, array_size);
	shader.setUniformArray("obj_color", obj_color, array_size);
	shader.setUniformArray("obj_name",  obj_name,  array_size);
	shader.setUniformArray("obj_size",  obj_size,  array_size);
	shader.setUniformArray("obj_ref",   obj_ref,   array_size);

	shader.setUniformArray("source_coord", source_coord, sources_number);
	shader.setUniformArray("source_color", source_color, sources_number);
	shader.setUniformArray("source_size",  source_size,  sources_number);
	shader.setUniform     ("sources_number",             sources_number);

	shader.setUniform("cam", Vector3f(objects[array_size]->obj));
	shader.setUniform("u_resolution", Vector2f(width, height));
}