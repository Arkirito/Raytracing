#include <iostream>
#include <fstream>

#include <random>
#include <time.h>

#include <vec3.h>
#include <ray.h>
#include <hitable.h>
#include <hitable_list.h>
#include <sphere.h>
#include <camera.h>

#define WIDTH 200
#define HEIGHT 100

vec3 GetRayProducedColor(const ray& r, hitable* world);
vec3 random_in_unit_sphere();
float getRundomFloat();

vec3 GetRayProducedColor(const ray& r, hitable* world)
{
	hit_record rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5f*GetRayProducedColor(ray(rec.p, target-rec.p), world);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0*vec3(getRundomFloat(),getRundomFloat(),getRundomFloat()) - vec3(1,1,1);
	} 
	while (p.squared_length() >= 1.f);

	return p;
}

float getRundomFloat()
{
	return (((double)rand() / (RAND_MAX)));
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

	camera cam;

	srand(time(NULL));

	for(int j = HEIGHT - 1; j >= 0; --j)
	{
		for(int i = 0; i <  WIDTH; ++i)
		{
			vec3 color(0, 0, 0);

			for (int s = 0; s < HEIGHT; s++)
			{
				float u = float(i + getRundomFloat()) / float(WIDTH);
				float v = float(j + getRundomFloat()) / float(HEIGHT);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.f);
				color += GetRayProducedColor(r, world);
			}

			color /= float(HEIGHT);
			color = vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

			int ir = int(255.99*color[0]);
			int ig = int(255.99*color[1]);
			int ib = int(255.99*color[2]);

			output << ir << " " << ig << " " << ib << "\n";
		}
	}

	output.close();
}