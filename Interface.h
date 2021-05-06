#pragma once
#include <sstream>
#include "sfml.h"

Sprite  sphere_icon;
Image	sphere_image;
Texture sphere_texture;

Sprite  cube_icon;
Image   cube_image;
Texture cube_texture;

Sprite  light_icon;
Image   light_image;
Texture light_texture;

Sprite  background_icon;
Image   background_image;
Texture background_texture;

Texture sky_texture;

float brightness = 1.0;
float bright_copy;

class Interface
{
public:
	Vector2f size = Vector2f(0, 0),
		     pos  = Vector2f(0, 0);

	int click_counter = 0;
	const char* name = "Interface";
	bool selected = false;

	virtual void draw()    = 0;
	virtual float action() = 0;
	virtual bool click()   = 0;
};

class button_add_sphere : public Interface
{
public:
	button_add_sphere(Vector2f size, Vector2f pos)
	{
		this->size = size;
		this->pos = pos;
		this->name = "sphere";
	}

	void draw() override
	{
		RectangleShape rectangle(size);
		rectangle.move(pos);
		if (click_counter % 2 == 0) rectangle.setFillColor(Color(83, 83, 83));
		else						rectangle.setFillColor(Color(59, 59, 59));

		sphere_icon.setPosition(pos);

		interface.draw(rectangle);
		interface.draw(sphere_icon);
	}

	float action() override { return -1; }

	bool click() override 
	{ 
		Vector2i mouse = Mouse::getPosition(interface);
		if (mouse.x >= pos.x && mouse.y >= pos.y && mouse.x <= pos.x + size.x && mouse.y <= pos.y + size.y && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			click_counter++;
			return true;
		}
		return false;
	}
};

class button_add_cube : public Interface
{
public:
	button_add_cube(Vector2f size, Vector2f pos)
	{
		this->size = size;
		this->pos = pos;
		this->name = "cube";
	}

	void draw() override
	{
		RectangleShape rectangle(size);
		rectangle.move(pos);
		if (click_counter % 2 == 0) rectangle.setFillColor(Color(83, 83, 83));
		else						rectangle.setFillColor(Color(59, 59, 59));

		cube_icon.setPosition(pos);

		interface.draw(rectangle);
		interface.draw(cube_icon);
	}

	float action() override { return -1; }

	bool click() override
	{
		Vector2i mouse = Mouse::getPosition(interface);
		if (mouse.x >= pos.x && mouse.y >= pos.y && mouse.x <= pos.x + size.x && mouse.y <= pos.y + size.y && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			click_counter++;
			return true;
		}
		return false;
	}
};

class button_add_light : public Interface
{
public:
	button_add_light(Vector2f size, Vector2f pos)
	{
		this->size = size;
		this->pos = pos;
		this->name = "light";
	}

	void draw() override
	{
		RectangleShape rectangle(size);
		rectangle.move(pos);
		if (click_counter % 2 == 0) rectangle.setFillColor(Color(83, 83, 83));
		else					    rectangle.setFillColor(Color(59, 59, 59)); 

		light_icon.setPosition(pos);

		interface.draw(rectangle);
		interface.draw(light_icon);
	}

	float action() override { return -1; }

	bool click() override
	{
		Vector2i mouse = Mouse::getPosition(interface);
		if (mouse.x >= pos.x && mouse.y >= pos.y && mouse.x <= pos.x + size.x && mouse.y <= pos.y + size.y && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			click_counter++;
			return true;
		}
		return false;
	}
};

class button_background : public Interface
{
public:
	button_background(Vector2f size, Vector2f pos)
	{
		this->size = size;
		this->pos = pos;
		this->name = "background";
	}

	void draw() override
	{
		RectangleShape rectangle(size);
		rectangle.move(pos);
		if (click_counter % 2 == 0) rectangle.setFillColor(Color(83, 83, 83));
		else					    rectangle.setFillColor(Color(59, 59, 59));

		background_icon.setPosition(pos + Vector2f(13, 13));

		interface.draw(rectangle);
		interface.draw(background_icon);
	}

	float action() override { return -1; }

	bool click() override
	{
		Vector2i mouse = Mouse::getPosition(interface);
		if (mouse.x >= pos.x && mouse.y >= pos.y && mouse.x <= pos.x + size.x && mouse.y <= pos.y + size.y && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			click_counter++;
			return true;
		}
		return false;
	}
};

class object_preview : public Interface
{
	Vector3f color         = Vector3f(0, 0, 0);
	Vector3f size_spec_ref = Vector3f(0, 0, 0);
	Vector2f phi           = Vector2f(0, 0);

	const char* object_name = "";

public:
	object_preview(Vector2f size, Vector2f pos)
	{
		this->size = size;
		this->pos = pos;
		this->name = "Object Preview";
	}

	void draw() override
	{
		Text name;
		Font font;
		font.loadFromFile("Fonts/arial.ttf");
		name.setFont(font);
		name.setString(object_name);
		name.setCharacterSize(24);
		name.move(Vector2f(pos.x + 10, pos.y - 30));

		RectangleShape rect(size);
		rect.setFillColor(Color(59, 59, 59));
		rect.move(pos);

		RenderTexture preview_texture;
		preview_texture.create(size.x - 20, size.y - 20);

		Sprite preview_sprite;
		preview_sprite = Sprite(preview_texture.getTexture());
		preview_sprite.move(Vector2f(pos.x + 10, pos.y + 10));

		Vector3f obj_color[2];
		float    obj_name [2];
		float    obj_size [2];
		Vector3f obj_prop [2]; 

		obj_color[0] = Vector3f(0.5, 0.5, 0.5);
		obj_name [0] = 3.0;
		obj_size [0] = 0;
		obj_prop [0] = Vector3f(0.0, 0.0, 0.0);
		obj_color[1] = Vector3f(color.x / 255, color.y / 255, color.z / 255);
		obj_size [1] = size_spec_ref.x;
		obj_prop [1] = Vector3f(size_spec_ref.y, size_spec_ref.z, 0);

		if (object_name == "sphere")  obj_name[1] = 1.0;
		if (object_name == "cube")    obj_name[1] = 2.0;
		if (object_name == "light") { obj_name[1] = 1.0; obj_prop[1] = Vector3f(0, 0, 0); }

		preview_shader.setUniformArray("obj_color", obj_color, 2);
		preview_shader.setUniformArray("obj_name",  obj_name,  2);
		preview_shader.setUniformArray("obj_size",  obj_size,  2);
		preview_shader.setUniformArray("obj_prop",  obj_prop,  2);

		preview_shader.setUniform("u_resolution", Vector2f(size.x - 20, size.y - 20));
		preview_shader.setUniform("phi", phi);
		preview_shader.setUniform("u_skybox", sky_texture);

		interface.draw(rect);
		interface.draw(name);
		interface.draw(preview_sprite, &preview_shader);
	}

	float action() override { return -1; }

	bool click() override 
	{
		static Vector2i coord1, coord2 = Mouse::getPosition(interface);

		coord1 = Mouse::getPosition(interface);
		if (coord1.x >= pos.x + 10 && coord1.x <= pos.x + size.x - 10 &&
			coord1.y >= pos.y + 10 && coord1.y <= pos.y + size.y - 10)
		{
			if (abs(coord2.x - coord1.x) > size.x / 8 || abs(coord2.y - coord1.y) > size.y / 8)
				coord2 = coord1;
			phi.x += (coord1.x - coord2.x) / (size.x - 20);
			phi.y += (coord1.y - coord2.y) / (size.y - 20);
			coord2 = coord1;
		}

		return false;
	}

	void set_obj_name(const char* str)
	{
		object_name = str;
	}

	void set_obj_color(Vector3f color)
	{
		this->color = color;
	}

	void set_obj_properties(Vector3f ssr)
	{
		this->size_spec_ref = ssr;
	}
};

class button_switch : public Interface
{
	float lenght = 0;
	int switch_pos = 0;
	Vector2f switch_size = Vector2f(6, 12);
	Vector2f value_size = Vector2f(60, 30);
	bool selected = false;

public:
	button_switch(Vector2f size, Vector2f pos, const char* name, float lenght, int k)
	{
		this->size = size;
		this->pos = pos;
		this->name = name;
		this->lenght = lenght;
		click_counter = k;
	}

	void draw() override
	{
		RectangleShape rect(size);
		rect.setFillColor(Color(59, 59, 59));
		rect.move(pos);

		Font font;
		font.loadFromFile("Fonts/arial.ttf");
		Text Name(name, font, 24);
		Name.move(Vector2f(pos.x + 5, pos.y + size.y / 2 - 12));

		RectangleShape line(Vector2f(size.x - 180, 4));
		line.setFillColor(Color(30, 30, 30));
		line.move(pos.x + 55, pos.y + size.y / 2 - 2);

		std::ostringstream ostr1;
		ostr1 << lenght;
		std::string len = ostr1.str();
		Text Len(len, font, 24);
		Len.move(Vector2f(pos.x + size.x - 120, pos.y + size.y / 2 - 12));

		RectangleShape swt(switch_size);
		swt.setFillColor(Color(83, 83, 83));
		swt.move(Vector2f(pos.x + 55 + switch_pos - switch_size.x / 2, pos.y + size.y / 2 - switch_size.y / 2));

		RectangleShape value(value_size);
		value.setFillColor(Color(83, 83, 83));
		value.move(Vector2f(pos.x + size.x - 70, pos.y + size.y / 2 - 15));
		
		std::ostringstream ostr2;
		ostr2 << rounding();
		std::string val = ostr2.str();
		Text Val(val, font, 24);
		Val.move(Vector2f(pos.x + size.x - 70, pos.y + size.y / 2 - 12));

		interface.draw(rect);
		interface.draw(Name);
		interface.draw(line);
		interface.draw(Len);
		interface.draw(swt);
		interface.draw(value);
		interface.draw(Val);
	}

	float action() override 
	{
		return lenght * switch_pos / (size.x - 180);
	}

	bool click() override 
	{
		Vector2i mouse = Mouse::getPosition(interface);
		if (mouse.x >= pos.x + 55 + switch_pos - switch_size.x / 2 && mouse.x <= pos.x + 55 + switch_pos + switch_size.x / 2 &&
			mouse.y >= pos.y + size.y / 2 - switch_size.y / 2 && mouse.y <= pos.y + size.y / 2 + switch_size.y / 2 &&
			Mouse::isButtonPressed(Mouse::Button::Left)) selected = true;

		if (mouse.x < pos.x || mouse.x > pos.x + size.x ||
			mouse.y < pos.y || mouse.y > pos.y + size.y) selected = false;
		
		if(selected)
		{
			switch_pos = mouse.x - pos.x - 55;
			if (switch_pos > size.x - 180) switch_pos = size.x - 180;
			if (switch_pos < 0) switch_pos = 0;
		}

		return false;
	}

	float rounding()
	{
		float number = action();
		const int numerals_amount = 3;
		int k = 0;

		for (int i = numerals_amount; i >= 0; i--)
		{
			if ((number / pow(10, i)) >= 1.0)
			{
				k = i;
				break;
			}
		}

		float num = int(number * pow(10, (numerals_amount - k)) + 0.5);

		return num / pow(10, (numerals_amount - k));
	}
};

class interface_manager
{
	std::vector<Interface*> buttons;
	object_preview* preview;

public:
	interface_manager()
	{
		preview =         new object_preview   (Vector2f(300, 300), Vector2f(  0, 225));
		buttons.push_back(new button_add_sphere(Vector2f( 96,  96), Vector2f(  3,   3)));
		buttons.push_back(new button_add_cube  (Vector2f( 96,  96), Vector2f(102,   3)));
		buttons.push_back(new button_add_light (Vector2f( 96,  96), Vector2f(201,   3)));
		buttons.push_back(new button_background(Vector2f( 96,  96), Vector2f(  3, 102)));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 560), "R: 0",  255, -2));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 613), "G: 0",  255, -2));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 666), "B: 0",  255, -2));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 751), "si: 0", 200, -6));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 804), "sp:0",  1.0, -4));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 857), "n: 0",  2.5, -4));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 560), "Br:0",  100, -8));
		buttons.push_back(preview);
	}

	void getIcons()
	{
		sphere_image.loadFromFile("Icons/sphereicon.png");
		sphere_image.createMaskFromColor(Color(255, 255, 255));
		sphere_texture.loadFromImage(sphere_image);
		sphere_icon.setTexture(sphere_texture);

		cube_image.loadFromFile("Icons/cubeicon.png");
		cube_image.createMaskFromColor(Color(255, 255, 255));
		cube_texture.loadFromImage(cube_image);
		cube_icon.setTexture(cube_texture);

		light_image.loadFromFile("Icons/lighticon.png");
		light_image.createMaskFromColor(Color(255, 255, 255));
		light_texture.loadFromImage(light_image);
		light_icon.setTexture(light_texture);

		background_image.loadFromFile("Icons/backgroundicon.png");
		background_image.createMaskFromColor(Color(255, 255, 255));
		background_texture.loadFromImage(background_image);
		background_icon.setTexture(background_texture);

		sky_texture.loadFromFile("Images/space1.jpeg");
	}

	void DrawInterface()
	{
		for (int i = 0; i < buttons.size(); i++)
		{ 
			Font font;
			font.loadFromFile("Fonts/arial.ttf");
			Text color("Color:", font, 24);
			Text properties("Properties:", font, 24);

			if (buttons[i]->click_counter >= 0) buttons[i]->draw();

			if (buttons[i]->selected && buttons[i]->name == "light")
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->click_counter == -2 || buttons[i]->click_counter == -6) buttons[i]->draw();
				color.move(100, 528);
				interface.draw(color);
				properties.move(80, 720);
				interface.draw(properties);
			}

			if (buttons[i]->selected && buttons[i]->name != "light" && buttons[i]->name != "background")
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->click_counter < 0 && buttons[i]->click_counter != -8) buttons[i]->draw();
				color.move(100, 528);
				interface.draw(color);
				properties.move(80, 720);
				interface.draw(properties);
			}

			if (buttons[i]->selected && buttons[i]->name == "background")
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->click_counter == -8) buttons[i]->draw();
				properties.move(100, 528);
				interface.draw(properties);
			}
		}
	}

	void ButtonPressed() 
	{
		static int number_pressed_button = 0;
		static Vector3f Color;
		static Vector3f SSR;
		int k1 = 0;
		int k2 = 0;

		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i]->click_counter >= 0)
			{
				if (buttons[i]->click())
				{
					if (buttons[i]->click_counter % 2 != 0)
					{
						buttons[i]->selected = true;
						preview->set_obj_name(buttons[i]->name);
					}
					else
					{
						buttons[i]->selected = false;
						preview->set_obj_name("");
					}
					number_pressed_button = i;
				}
			}

			if (buttons[i]->selected && buttons[i]->name != "background")
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->click_counter == -2)
					{
						buttons[i]->click();
						k1++;
						if (k1 == 1) Color.x = buttons[i]->action();
						if (k1 == 2) Color.y = buttons[i]->action();
						if (k1 == 3) Color.z = buttons[i]->action();
					}
			}
		
			if (buttons[i]->selected && buttons[i]->name != "background")
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->click_counter == -4 || buttons[i]->click_counter == -6)
					{
						buttons[i]->click();
						k2++;
						if (k2 == 1) SSR.x = buttons[i]->action();
						if (k2 == 2) SSR.y = buttons[i]->action();
						if (k2 == 3) SSR.z = buttons[i]->action();
					}
			}

			if (buttons[i]->selected && buttons[i]->name == "background")
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->click_counter == -8)
					{
						buttons[i]->click();
						brightness = 1.0 + 1.5 * buttons[i]->action() / 100;
					}
			}
		}

		preview->set_obj_color(Color);
		preview->set_obj_properties(SSR);

		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i]->click_counter % 2 != 0 && i != number_pressed_button)
			{
				buttons[i]->selected = false;
				buttons[i]->click_counter--;
			}
		}
	}

	~interface_manager()
	{
		delete preview;
	}
};