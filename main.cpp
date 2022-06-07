#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <conio.h>
#include <math.h>

typedef struct {
	int state_count_down;
	int state;
	sf::RectangleShape texture;
} entity;

#define PI 3.1412

//define the function

inline float f(float x) { return 1. / 2. * x * x + sinf(PI*x); }
inline float df(float x) { return 1. / 1. * x + PI * cosf(PI*x); }
inline float ddf(float x) { return 1. / 1. - PI * PI * sinf(PI*x); }

//inline float f(float x)   { return 1. / 16. * x * x * x * x + 1. / 8. * x * x + sinf(PI*x);  }
//inline float df(float x)  { return 1. / 4. * x * x * x + 1. / 4. * x + PI * cosf(PI*x); }
//inline float ddf(float x) {	return 3. / 4. * x * x + 1. / 4. - PI * PI * sinf(PI*x);}

//inline float f(float x)   { return sinf(x * x) + 2 * x * x * cosf(x * x);  }
//inline float df(float x)  { return 6 * x * cosf(x * x) - 4 * x * x * x * sinf(x * x); }
//inline float ddf(float x) {	return 6 * cosf(x * x) - 24 * x * x * sinf(x * x) - 8 * x * x * x * x * cosf(x * x);}

//inline float f(float x)   { return x * sinf(x * x) + 1;  }
//inline float df(float x)  { return sinf(x * x) + 2 * x * x * cosf(x * x); }
//inline float ddf(float x) {	return 6 * x * cosf(x * x) - 4 * x * x * x * sinf(x * x);}

void draw_function(float (*func)(float), float lower, float upper, float step, float scale_x, float scale_y, float origin_x, float origin_y, sf::Color color, sf::RenderWindow& window) {
	sf::Vertex line[2];
	float x = lower;
	int init = 1;
	while (x <= upper) {
		float y = func(x);
		if (init) {
			init = 0;
			line[0] = sf::Vector2f(origin_x + scale_x * x, origin_y - scale_y * y);
			line[0].color = color;
		}
		else {
			line[0] = line[1];
		}
		line[1] = sf::Vector2f(origin_x + scale_x * x, origin_y - scale_y * y);
		line[1].color = color;
		x += step;
		window.draw(line, 2, sf::Lines);
	}
}

int main() {
	float lower = -20.f;
	float upper =  20.f;
	float step = 0.01f;
	float scale_x = 40;
	float scale_y = 40;

	//env
	float ww = 2000;
	float wh = 1000;
	float horz = 2.f * wh / 3.f;
	float vert = ww / 2.f;

	int unset_gx = 1;
	float gx;

	float x = 0;
	float y;
	float dx;
	float ddx;

	int init;

	sf::Vertex line[2];
	sf::RenderWindow window(sf::VideoMode(ww, wh), "graph interactive");
	sf::Color white = sf::Color(255, 255, 255, 255);
	sf::Color origin_color = sf::Color(255, 255, 255, 128);
	sf::Color y_color = sf::Color(64, 128, 200, 255);
	sf::Color dx_color = sf::Color(200, 128, 64, 128);
	sf::Color ddx_color = sf::Color(128, 200, 64, 64);

	sf::Font font;
	font.loadFromFile("Roboto-Regular.ttf");

	float char_size = 16;

	char buf[256];
	sf::Text info;
	info.setFont(font);
	info.setCharacterSize(char_size);

	float point_radius = 3.f;
	sf::CircleShape q;
	q.setRadius(point_radius);
	q.setPosition(0, 0);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::MouseMoved) {
				x = (event.mouseMove.x - vert) / scale_x;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				if (event.key.code == sf::Keyboard::R) {
					unset_gx = 1;
				}
			}
		}

		window.clear();

		line[0] = sf::Vector2f(0, horz);
		line[1] = sf::Vector2f(ww-1, horz);
		line[0].color = origin_color;
		line[1].color = origin_color;
		window.draw(line, 2, sf::Lines);

		line[0] = sf::Vector2f(vert, 0);
		line[1] = sf::Vector2f(vert, wh-1);
		line[0].color = origin_color;
		line[1].color = origin_color;
		window.draw(line, 2, sf::Lines);

		draw_function(f, lower, upper, step, scale_x, scale_y, vert, horz, y_color, window);
		draw_function(df, lower, upper, step, scale_x, scale_y, vert, horz, dx_color, window);
		draw_function(ddf, lower, upper, step, scale_x, scale_y, vert, horz, ddx_color, window);

		sprintf(buf, "x = %f", x);
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 0);
		q.setFillColor(white);
		window.draw(info);

		sprintf(buf, "y = %f", f(x));
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 1);
		window.draw(info);
		q.setPosition(vert + x * scale_x - point_radius, horz - scale_y * f(x) - point_radius);
		q.setOutlineColor(y_color);
		window.draw(q);

		sprintf(buf, "dx = %f", df(x));
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 2);
		window.draw(info);
		q.setPosition(vert + x * scale_x - point_radius, horz - scale_y * df(x) - point_radius);
		q.setOutlineColor(dx_color);
		window.draw(q);

		sprintf(buf, "ddx = %f", ddf(x));
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 3);
		window.draw(info);
		q.setPosition(vert + x * scale_x - point_radius, horz - scale_y * ddf(x) - point_radius);
		q.setOutlineColor(ddx_color);
		window.draw(q);

		if (unset_gx) {
			gx = x;
			unset_gx = 0;
		}
		else {
			if (f(x) < f(gx)) {
				gx = x;
			}
		}

		sprintf(buf, "x = %f", gx);
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 6);
		window.draw(info);

		sprintf(buf, "y = %f", f(gx));
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 7);
		window.draw(info);
		q.setPosition(vert + gx * scale_x - point_radius, horz - scale_y * f(gx) - point_radius);
		q.setFillColor(y_color);
		window.draw(q);

		sprintf(buf, "dx = %f", df(gx));
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 8);
		window.draw(info);
		q.setPosition(vert + gx * scale_x - point_radius, horz - scale_y * df(gx) - point_radius);
		q.setFillColor(dx_color);
		window.draw(q);

		sprintf(buf, "ddx = %f", ddf(gx));
		info.setString(buf);
		info.setPosition(10, 10 + char_size * 9);
		window.draw(info);
		q.setPosition(vert + gx * scale_x - point_radius, horz - scale_y * ddf(gx) - point_radius);
		q.setFillColor(ddx_color);
		window.draw(q);


		window.display();
	}

	return 0;
}