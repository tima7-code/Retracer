#pragma once
#include "sfml.h"

class object
{
public:

	int size = 0;
	Vector3f properties = Vector3f(0.0, 0.0, 0.0);
	Vector3f obj =        Vector3f(0.0, 0.0, 0.0);
	Vector3f color =      Vector3f(1.0, 1.0, 1.0);
	const char* name = "name";

	virtual bool move() = 0;
	virtual bool rotation() = 0;
};

class sphere : public object
{
public:
	sphere(Vector3f coord, Vector3f RGB, int radius, Vector3f RNM)
	{
		name = "sphere";
		size = radius;
		obj = Vector3f(coord.x, -coord.y, coord.z);
		color = Vector3f(RGB.x / 255, RGB.y / 255, RGB.z / 255);

		if (RNM.x > 1.0)      this->properties.x = 1.0;
		else if (RNM.x < 0.0) this->properties.x = 0.0;
		else                  this->properties.x = RNM.x;

		if (RNM.y < 0.0)      this->properties.y = 0.0;
		else                  this->properties.y = RNM.y;
	}

	bool move() override { return false; }

	bool rotation() override { return false; }
};

class cube : public object
{
public:
	cube(Vector3f coord, Vector3f RGB, int edge, Vector3f RNM)
	{
		name = "cube";
		size = edge;
		obj = Vector3f(coord.x, -coord.y, coord.z);
		color = Vector3f(RGB.x / 255, RGB.y / 255, RGB.z / 255);

		if (RNM.x > 1.0)      this->properties.x = 1.0;
		else if (RNM.x < 0.0) this->properties.x = 0.0;
		else                  this->properties.x = RNM.x;

		if (RNM.y < 0.0)      this->properties.y = 0.0;
		else                  this->properties.y = RNM.y;
	}

	bool move() override { return false; }

	bool rotation() override { return false; }
};

class plane : public object
{
public:
	plane(Vector3f normal, Vector3f RGB, Vector3f RNM)
	{
		name = "plane";
		obj = Vector3f(normal.x, -normal.y, normal.z);
		color = Vector3f(RGB.x / 255, RGB.y / 255, RGB.z / 255);

		if (RNM.x > 1.0)      this->properties.x = 1.0;
		else if (RNM.x < 0.0) this->properties.x = 0.0;
		else                  this->properties.x = RNM.x;

		if (RNM.y < 0.0)      this->properties.y = 0.0;
		else                  this->properties.y = RNM.y;
	}

	bool move() override { return false; }

	bool rotation() override { return false; }
};

class light : public object
{
public:
	light(Vector3f coord, Vector3f RGB, int radius)
	{
		name = "light";
		size = radius;
		obj = Vector3f(coord.x, -coord.y, coord.z);
		color = Vector3f(RGB.x / 255, RGB.y / 255, RGB.z / 255);
	}

	bool move() override { return false; }

	bool rotation() override { return false; }
};

class camera : public object
{
	Vector2f phi;
	float speed = 2.0;
	float mouse_sensitivity = 2.0;

public:

	camera(Vector3f coord)
	{
		obj = Vector3f(coord.x, -coord.y, coord.z);
	}

	bool move() override
	{
		static Vector3f route;
		static Vector3f route_temp;
		route = Vector3f(0.0, 0.0, 0.0);
		if (Keyboard::isKeyPressed(Keyboard::W)) route.z -= 1.0;
		if (Keyboard::isKeyPressed(Keyboard::A)) route.x -= 1.0;
		if (Keyboard::isKeyPressed(Keyboard::S)) route.z += 1.0;
		if (Keyboard::isKeyPressed(Keyboard::D)) route.x += 1.0;
		route_temp.z = route.z * cos(-phi.y) - route.y * sin(-phi.y);
		route_temp.y = route.z * sin(-phi.y) + route.y * cos(-phi.y);
		route_temp.x = route.x;
		route.x = route_temp.x * cos(phi.x) - route_temp.z * sin(phi.x);
		route.z = route_temp.x * sin(phi.x) + route_temp.z * cos(phi.x);
		route.y = route_temp.y;
		obj += route * speed;
		if (route != Vector3f(0.0, 0.0, 0.0)) return true;
		else                                  return false;
	}

	bool rotation() override
	{
		static Vector2f displace = Vector2f(0.0, 0.0);
		static Vector2f disp;
		static Vector2i mouse;
		mouse = Mouse::getPosition(window);
		Mouse::setPosition(Vector2i(width / 2, height / 2), window);
		disp = displace;
		displace.x += mouse.x - width / 2;
		displace.y += mouse.y - height / 2;
		phi.x = displace.x / width;
		phi.y = displace.y / height;
		phi *= mouse_sensitivity;
		shader.setUniform("mouse", Vector2f(phi.x, phi.y));
		if (disp != displace) return true;
		else                  return false;
	}
};