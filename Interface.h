#pragma once
#include <sstream>
#include "sfml.h"

Sprite  sphere_icon;
Image	sphere_image;
Texture sphere_texture;

Sprite  cube_icon;
Image   cube_image;
Texture cube_texture;

Sprite light_icon;
Image light_image;
Texture light_texture;

class Interface
{
public:
	Vector2f size = Vector2f(0, 0),
		     pos  = Vector2f(0, 0);

	int click_counter = 0;
	const char* name = "Interface";

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

class object_preview : public Interface
{
	Vector3f color = Vector3f(0, 0, 0);
	const char* object_name = "";
	int object_size = 100;

public:
	object_preview(Vector2f size, Vector2f pos)
	{
		this->size = size;
		this->pos = pos;
		this->name = "Object Preview";
	}

	void draw() override
	{
		RectangleShape rectangle(size);
		rectangle.move(pos);
		rectangle.setFillColor(Color(59, 59, 59));

		RectangleShape rect1(Vector2f(size.x - 20, size.y / 2 - 10));
		rect1.move(Vector2f(pos.x + 10, pos.y + 10));
		rect1.setFillColor(Color(0, 191, 255));

		RectangleShape rect2(Vector2f(size.x - 20, size.y / 2 - 10));
		rect2.move(Vector2f(pos.x + 10, pos.y + size.y / 2));
		rect2.setFillColor(Color(125, 125, 125));

		CircleShape sphere;
		RectangleShape cube;
		CircleShape light;
		Text name;
		Font font;
		font.loadFromFile("Fonts/arial.ttf");
		name.setFont(font);
		name.setString(object_name);
		name.setCharacterSize(24);
		name.move(Vector2f(pos.x + 10, pos.y - 30));

		if (object_name == "sphere")
		{
			sphere.setRadius(object_size / 2);
			sphere.setFillColor(Color(color.x, color.y, color.z));
			sphere.move(Vector2f(pos.x + size.x / 3, pos.y + size.x / 3));
		}
		if (object_name == "cube")
		{
			cube.setSize(Vector2f(object_size, object_size));
			cube.setFillColor(Color(color.x, color.y, color.z));
			cube.move(Vector2f(pos.x + size.x / 3, pos.y + size.x / 3));
		}
		if (object_name == "light")
		{
			light.setRadius(object_size / 2);
			light.setFillColor(Color(color.x, color.y, color.z));
			light.move(Vector2f(pos.x + size.x / 3, pos.y + size.x / 3));
		}

		interface.draw(rectangle);
		interface.draw(rect1);
		interface.draw(rect2);

		interface.draw(sphere);
		interface.draw(cube);
		interface.draw(light);
		interface.draw(name);
	}

	float action() override { return -1; }

	bool click() override { return false; }

	void set_obj_name(const char* str)
	{
		object_name = str;
	}

	void set_obj_color(Vector3f color)
	{
		this->color = color;
	}
};

class button_switch : public Interface
{
	float lenght = 0;
	int switch_pos = 4;
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
		preview =         new object_preview   (Vector2f(300, 300), Vector2f(  0, 201));
		buttons.push_back(new button_add_sphere(Vector2f( 96,  96), Vector2f(  3,   3)));
		buttons.push_back(new button_add_cube  (Vector2f( 96,  96), Vector2f(102,   3)));
		buttons.push_back(new button_add_light (Vector2f( 96,  96), Vector2f(201,   3)));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 504), "R: 0", 255, -2));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 557), "G: 0", 255, -2));
		buttons.push_back(new button_switch    (Vector2f(294,  50), Vector2f(  3, 610), "B: 0", 255, -2));
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
	}

	void DrawInterface()
	{
		for (int i = 0; i < buttons.size(); i++)
		{ 
			buttons[i]->draw();
		}
	}

	void ButtonPressed() 
	{
		static int number_pressed_button = 0;
		static Vector3f Color;
		int k = 0;

		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i]->click())
			{
				if (buttons[i]->click_counter % 2 != 0) preview->set_obj_name(buttons[i]->name);
				else                                    preview->set_obj_name("");
				number_pressed_button = i;
			}

			if (buttons[i]->click_counter == -2)
			{
				k++;
				if (k == 1) Color.x = buttons[i]->action();
				if (k == 2) Color.y = buttons[i]->action();
				if (k == 3) Color.z = buttons[i]->action();
			}
		}

		preview->set_obj_color(Color);

		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i]->click_counter % 2 != 0 && i != number_pressed_button)
				buttons[i]->click_counter--;
		}
	}

	~interface_manager()
	{
		delete preview;
	}
};