#include <iostream>
#include <fstream>

#include <vec3.h>
#include <ray.h>

#define WIDTH 200
#define HEIGHT 100

float hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0)
	{
		return -1.f;
	}
	else
	{
		return (-b - sqrt(discriminant)) / (2.0f*a);
	}
}

vec3 blendedColor(const ray& r)
{
	float t = hit_sphere(vec3(0, 0, -1), 0.5f, r);
	if (t > 0)
	{
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0.f, 0.f, -1.f));
		return 0.5f*vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5f*(unit_direction.y() + 1.0f);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	std::ofstream output("output.ppm", std::ofstream::out);

	output << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
	
	vec3 lower_left_corner(-2.0f, -1.0f, -1.0);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);

	for(int j = HEIGHT - 1; j >= 0; --j)
	{
		for(int i = 0; i <  WIDTH; ++i)
		{
			float u = float(i) / float(WIDTH);
			float v = float(j) / float(HEIGHT);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 color = blendedColor(r);

			int ir = int(255.99*color[0]);
			int ig = int(255.99*color[1]);
			int ib = int(255.99*color[2]);

			output << ir << " " << ig << " " << ib << "\n";
		}
	}

	output.close();
}