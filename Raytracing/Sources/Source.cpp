#include <iostream>
#include <fstream>

#include <vec3.h>
#include <ray.h>
#include <hitable.h>
#include <hitable_list.h>
#include <sphere.h>
#include <camera.h>
#include <material.h>

#include <common.h>

#define WIDTH 600
#define HEIGHT 300

vec3 GetRayProducedColor(const ray& r, hitable* world, int depth);

vec3 GetRayProducedColor(const ray& r, hitable* world, int depth)
{
	hit_record rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * GetRayProducedColor(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
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

	hitable *list[4];
	list[0] = new sphere(vec3(0.f, 0.f, -1.f), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(0.f, -100.5f, -1.f), 100, new lambertian(vec3(0.8, 0.8, 0.3)));
	list[2] = new sphere(vec3(1.f,  0.f, -1.f), 0.5f, new metal(vec3(0.8, 0.8, 0.3)));
	list[3] = new sphere(vec3(-1.f, 0.f, -1.f), 0.5f, new dielectric(1.5f));
	hitable *world = new hitable_list(list, 4);

	camera cam(vec3(-1.5, 1.5, 1), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(WIDTH)/float(HEIGHT));

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
				color += GetRayProducedColor(r, world, 0);
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