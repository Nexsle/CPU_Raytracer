#include "rtweekend.h"

#include "Hittable.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

int main() 
{
	hittableList world;

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.Render(world);
}