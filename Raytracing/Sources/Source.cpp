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

#define WIDTH 100
#define HEIGHT 50

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

hitable* random_scene()
{
	int n = 500;
	hitable** list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for(int a = -11; a < 11; a++)
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = getRundomFloat();
			vec3 center(a + 0.9*getRundomFloat(), 0.2f, b + 0.9f*getRundomFloat());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9f)
			{
				if (choose_mat < 0.8f) // diffuse
				{
					list[i++] = new sphere(center, 0.2f, new lambertian(vec3(getRundomFloat()*getRundomFloat(), getRundomFloat()*getRundomFloat(), getRundomFloat()*getRundomFloat())));
				}
				else if (choose_mat < 0.95f) // metal
				{
					list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f*(1 + getRundomFloat()), 0.5f*(1 + getRundomFloat()), 0.5f*(1 + getRundomFloat())), 0.5f*getRundomFloat()));
				}
				else //glass
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0f, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0f, new dielectric(1.5));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0f, new metal(vec3(0.588f, 0.737f, 0.462f), 0.0f));

	return new hitable_list(list, i);
}

int main()
{
	std::ofstream output("output.ppm", std::ofstream::out);

	output << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
	
	vec3 lower_left_corner(-2.0f, -1.0f, -1.0);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	hitable *world = random_scene();

	vec3 lookfrom(3, 3, 2);
	vec3 lookat(10, 0, -1);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 2.0f;

	camera cam(vec3(-5.5f, 2.5f, 5.5f), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(WIDTH)/float(HEIGHT), aperture, dist_to_focus);

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