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

Sprite  background1_icon;
Image   background1_image;
Texture background1_texture;

Sprite  background2_icon;
Image   background2_image;
Texture background2_texture;

Sprite  background3_icon;
Image   background3_image;
Texture background3_texture;

Sprite  background4_icon;
Image   background4_image;
Texture background4_texture;

Sprite  background5_icon;
Image   background5_image;
Texture background5_texture;

Texture sky_texture;
Texture background1;
Texture background2;
Texture background3;
Texture background4;
Texture background5;

float brightness = 1.0;
float bright_copy;

bool sky_texture_changed = false;

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

class button_add_background : public Interface
{
	int background_num = 0;

public:
	button_add_background(Vector2f size, Vector2f pos, int number)
	{
		this->size = size;
		this->pos = pos;
		this->name = "background";
		background_num = number;
	}

	void draw() override
	{
		RectangleShape rectangle(size);
		rectangle.move(pos);
		if (click_counter % 2 == 0) rectangle.setFillColor(Color(83, 83, 83));
		else					    rectangle.setFillColor(Color(59, 59, 59));

		if (background_num == 1) background1_icon.setPosition(pos + Vector2f(13, 13));
		if (background_num == 2) background2_icon.setPosition(pos + Vector2f(13, 13));
		if (background_num == 3) background3_icon.setPosition(pos + Vector2f(13, 13));
		if (background_num == 4) background4_icon.setPosition(pos + Vector2f(13, 13));
		if (background_num == 5) background5_icon.setPosition(pos + Vector2f(13, 13));

		interface.draw(rectangle);

		if (background_num == 1) interface.draw(background1_icon);
		if (background_num == 2) interface.draw(background2_icon);
		if (background_num == 3) interface.draw(background3_icon);
		if (background_num == 4) interface.draw(background4_icon);
		if (background_num == 5) interface.draw(background5_icon);
	}

	float action() override 
	{
		if (background_num == 1 && sky_texture_changed)
		{
			Vector2u size = background1.getSize();
			sky_texture.create(size.x, size.y);
			sky_texture.update(background1);
		}
		if (background_num == 2 && sky_texture_changed)
		{
			Vector2u size = background2.getSize();
			sky_texture.create(size.x, size.y);
			sky_texture.update(background2);
		}
		if (background_num == 3 && sky_texture_changed)
		{
			Vector2u size = background3.getSize();
			sky_texture.create(size.x, size.y);
			sky_texture.update(background3);
		}
		if (background_num == 4 && sky_texture_changed)
		{
			Vector2u size = background4.getSize();
			sky_texture.create(size.x, size.y);
			sky_texture.update(background4);
		}
		if (background_num == 5 && sky_texture_changed)
		{
			Vector2u size = background5.getSize();
			sky_texture.create(size.x, size.y);
			sky_texture.update(background5);
		}

		return -1;
	}

	bool click() override
	{
		Vector2i mouse = Mouse::getPosition(interface);
		if (mouse.x >= pos.x && mouse.y >= pos.y && mouse.x <= pos.x + size.x && mouse.y <= pos.y + size.y && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			click_counter++;
			sky_texture_changed = true;
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
	std::vector<Interface*> windows;
	std::vector<Interface*> sphere_buttons;
	std::vector<Interface*> cube_buttons;
	std::vector<Interface*> light_buttons;
	std::vector<Interface*> background_buttons;
	
	object_preview* preview;

public:
	interface_manager()
	{
		preview =         new object_preview   (Vector2f(300, 300), Vector2f(  0, 225));
		windows.push_back(new button_add_sphere(Vector2f( 96,  96), Vector2f(  3,   3)));
		windows.push_back(new button_add_cube  (Vector2f( 96,  96), Vector2f(102,   3)));
		windows.push_back(new button_add_light (Vector2f( 96,  96), Vector2f(201,   3)));
		windows.push_back(new button_background(Vector2f( 96,  96), Vector2f(  3, 102)));
		windows.push_back(preview);

		sphere_buttons.push_back(new button_switch(Vector2f(294,  50), Vector2f(  3, 560), "R: 0",  255, -2));
		sphere_buttons.push_back(new button_switch(Vector2f(294,  50), Vector2f(  3, 613), "G: 0",  255, -2));
		sphere_buttons.push_back(new button_switch(Vector2f(294,  50), Vector2f(  3, 666), "B: 0",  255, -2));
		sphere_buttons.push_back(new button_switch(Vector2f(294,  50), Vector2f(  3, 751), "si: 0", 200, -4));
		sphere_buttons.push_back(new button_switch(Vector2f(294,  50), Vector2f(  3, 804), "sp:0",  1.0, -4));
		sphere_buttons.push_back(new button_switch(Vector2f(294,  50), Vector2f(  3, 857), "n: 0",  2.5, -4));

		cube_buttons.push_back  (new button_switch(Vector2f(294,  50), Vector2f(  3, 560), "R: 0",  255, -2));
		cube_buttons.push_back  (new button_switch(Vector2f(294,  50), Vector2f(  3, 613), "G: 0",  255, -2));
		cube_buttons.push_back  (new button_switch(Vector2f(294,  50), Vector2f(  3, 666), "B: 0",  255, -2));
		cube_buttons.push_back  (new button_switch(Vector2f(294,  50), Vector2f(  3, 751), "si: 0", 200, -4));
		cube_buttons.push_back  (new button_switch(Vector2f(294,  50), Vector2f(  3, 804), "sp:0",  1.0, -4));
		cube_buttons.push_back  (new button_switch(Vector2f(294,  50), Vector2f(  3, 857), "n: 0",  2.5, -4));

		light_buttons.push_back (new button_switch(Vector2f(294,  50), Vector2f(  3, 560), "R: 0",  255, -2));
		light_buttons.push_back (new button_switch(Vector2f(294,  50), Vector2f(  3, 613), "G: 0",  255, -2));
		light_buttons.push_back (new button_switch(Vector2f(294,  50), Vector2f(  3, 666), "B: 0",  255, -2));
		light_buttons.push_back (new button_switch(Vector2f(294,  50), Vector2f(  3, 751), "si: 0", 200, -4));

		background_buttons.push_back(new button_switch        (Vector2f(294,  50), Vector2f(  3, 560), "Br:0",  100, -8));
		background_buttons.push_back(new button_add_background(Vector2f( 96,  96), Vector2f(  3, 613), 1));
		background_buttons.push_back(new button_add_background(Vector2f( 96,  96), Vector2f(102, 613), 2));
		background_buttons.push_back(new button_add_background(Vector2f( 96,  96), Vector2f(201, 613), 3));
		background_buttons.push_back(new button_add_background(Vector2f( 96,  96), Vector2f(  3, 712), 4));
		background_buttons.push_back(new button_add_background(Vector2f( 96,  96), Vector2f(102, 712), 5));
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

		background1_image.loadFromFile("Icons/background1icon.png");
		background1_image.createMaskFromColor(Color(255, 255, 255));
		background1_texture.loadFromImage(background1_image);
		background1_icon.setTexture(background1_texture);

		background2_image.loadFromFile("Icons/background2icon.png");
		background2_image.createMaskFromColor(Color(255, 255, 255));
		background2_texture.loadFromImage(background2_image);
		background2_icon.setTexture(background2_texture);

		background3_image.loadFromFile("Icons/background3icon.png");
		background3_image.createMaskFromColor(Color(255, 255, 255));
		background3_texture.loadFromImage(background3_image);
		background3_icon.setTexture(background3_texture);

		background4_image.loadFromFile("Icons/background4icon.png");
		background4_image.createMaskFromColor(Color(255, 255, 255));
		background4_texture.loadFromImage(background4_image);
		background4_icon.setTexture(background4_texture);

		background5_image.loadFromFile("Icons/background5icon.png");
		background5_image.createMaskFromColor(Color(255, 255, 255));
		background5_texture.loadFromImage(background5_image);
		background5_icon.setTexture(background5_texture);

		sky_texture.loadFromFile("Images/space1.jpeg");
		background1.loadFromFile("Images/space1.jpeg");
		background2.loadFromFile("Images/space2.jpeg");
		background3.loadFromFile("Images/space3.jpeg");
		background4.loadFromFile("Images/space4.jpeg");
		background5.loadFromFile("Images/space5.jpeg");
	}

	void DrawInterface()
	{
		Font font;
		font.loadFromFile("Fonts/arial.ttf");
		Text color("Color:", font, 24);
		Text properties("Properties:", font, 24);

		for (int i = 0; i < windows.size(); i++)
		{
			windows[i]->draw();

			if (windows[i]->selected && windows[i]->name == "sphere")
			{
				for (int j = 0; j < sphere_buttons.size(); j++) sphere_buttons[j]->draw();
				color.move(100, 528);
				interface.draw(color);
				properties.move(80, 720);
				interface.draw(properties);
			}

			if (windows[i]->selected && windows[i]->name == "cube")
			{
				for (int j = 0; j < cube_buttons.size(); j++) cube_buttons[j]->draw();
				color.move(100, 528);
				interface.draw(color);
				properties.move(80, 720);
				interface.draw(properties);
			}

			if (windows[i]->selected && windows[i]->name == "light")
			{
				for (int j = 0; j < light_buttons.size(); j++) light_buttons[j]->draw();
				color.move(100, 528);
				interface.draw(color);
				properties.move(80, 720);
				interface.draw(properties);
			}

			if (windows[i]->selected && windows[i]->name == "background")
			{
				for (int j = 0; j < background_buttons.size(); j++) background_buttons[j]->draw();
				properties.move(80, 528);
				interface.draw(properties);
			}
		}
	}

	void ButtonPressed() 
	{
		static int number_pressed_window = 0;
		static int number_pressed_button = 0;
		static Vector3f Color;
		static Vector3f SSR;
		int k1 = 0;
		int k2 = 0;
		static std::vector<Interface*> buttons;

		for (int i = 0; i < windows.size(); i++)
		{
			if (windows[i]->click())
			{
				if (windows[i]->click_counter % 2 != 0)
				{
					windows[i]->selected = true;
					preview->set_obj_name(windows[i]->name);
				}
				else
				{
					windows[i]->selected = false;
					preview->set_obj_name("");
				}
				number_pressed_window = i;
			}

			if (windows[i]->selected)
			{
				Color = Vector3f(0, 0, 0);
				SSR = Vector3f(0, 0, 0);
				buttons = button_selection(windows[i]->name);
				for (int j = 0; j < buttons.size(); j++)
				{
					if (buttons[j]->click())
					{
						buttons[j]->action();
						if (buttons[j]->click_counter % 2 != 0) buttons[j]->selected = true;
						else                                    buttons[j]->selected = false;
						number_pressed_button = j;
					}

					if (buttons[j]->click_counter == -2)
					{
						k1++;
						if (k1 == 1) Color.x = buttons[j]->action();
						if (k1 == 2) Color.y = buttons[j]->action();
						if (k1 == 3) Color.z = buttons[j]->action();
					}

					if (buttons[j]->click_counter == -4)
					{
						k2++;
						if (k2 == 1) SSR.x = buttons[j]->action();
						if (k2 == 2) SSR.y = buttons[j]->action();
						if (k2 == 3) SSR.z = buttons[j]->action();
					}

					if (buttons[j]->click_counter == -8)
					{
						brightness = 1.0 + 1.5 * buttons[j]->action() / 100;
					}
				}

				preview->set_obj_color(Color);
				preview->set_obj_properties(SSR);

				for (int j = 0; j < buttons.size(); j++)
				{
					if (buttons[j]->click_counter % 2 != 0 && j != number_pressed_button)
					{
						buttons[j]->selected = false;
						buttons[j]->click_counter--;
					}
				}
			}
		}

		for (int i = 0; i < windows.size(); i++)
		{
			if (windows[i]->click_counter % 2 != 0 && i != number_pressed_window)
			{
				windows[i]->selected = false;
				windows[i]->click_counter--;
			}
		}
	}

	std::vector<Interface*> & button_selection(const char* name)
	{
		if (name == "sphere")     return sphere_buttons;
		if (name == "cube")       return cube_buttons;
		if (name == "light")      return light_buttons;
		if (name == "background") return background_buttons;
	}

	~interface_manager()
	{
		delete preview;
	}
};