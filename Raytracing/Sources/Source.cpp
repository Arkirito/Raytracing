#include <iostream>
#include <fstream>

#include <vec3.h>
#include <ray.h>
#include <hitable.h>
#include <hitable_list.h>
#include <sphere.h>

#define WIDTH 200
#define HEIGHT 100

vec3 blendedColor(const ray& r, hitable* world)
{
	hit_record rec;

	if (world->hit(r, 0.0f, FLT_MAX, rec))
	{
		return 0.5f*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{
	std::ofstream output("output.ppm", std::ofstream::out);

	output << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
	
	vec3 lower_left_corner(-2.0f, -1.0f, -1.0);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable *list[2];
	list[0] = new sphere(vec3(0.f, 0.f, -1.f), 0.5);
	list[1] = new sphere(vec3(0.f, -100.5f, -1.f), 100);
	hitable *world = new hitable_list(list, 2);

	for(int j = HEIGHT - 1; j >= 0; --j)
	{
		for(int i = 0; i <  WIDTH; ++i)
		{
			float u = float(i) / float(WIDTH);
			float v = float(j) / float(HEIGHT);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 color = blendedColor(r, world);

			int ir = int(255.99*color[0]);
			int ig = int(255.99*color[1]);
			int ib = int(255.99*color[2]);

			output << ir << " " << ig << " " << ib << "\n";
		}
	}

	output.close();
}