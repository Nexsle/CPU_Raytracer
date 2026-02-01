#include "rtweekend.h"

#include "Hittable.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"

void BouncingSpheres() 
{
	hittableList world;
	
	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto chooseMat = RandomDouble();
			point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

			if ((center - point3(4, 0.2, 0)).Length() > 0.9)
			{
				if (chooseMat < 0.8)
				{
					//diffuse
					auto albedo = color::random() * color::random();
					auto sphereMaterial = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, RandomDouble(0, 0.5), 0);
					world.add(make_shared<sphere>(center, center2, 0.2, sphereMaterial));
				}
				else if (chooseMat < 0.95)
				{
					//metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = RandomDouble(0, 0.5);
					auto sphereMaterial = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					auto sphereMaterial = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	world = hittableList(make_shared<bvhNode>(world));

	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = point3(13, 2, 3);
	cam.lookAt = point3(0, 0,0);
	cam.viewUp = point3(0, 1, 0);
	
	cam.defocusAngle = 0.6;
	cam.focusDist = 10.0;

	cam.Render(world);
}

void CheckeredSpheres()
{
	hittableList world;
	auto checkerPattern = make_shared<checkerTexture>(0.3, color(.2, .3, .1), color(.9, .9, .9));

	world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checkerPattern)));
	world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checkerPattern)));


	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = point3(13, 2, 3);
	cam.lookAt = point3(0, 0, 0);
	cam.viewUp = point3(0, 1, 0);

	cam.defocusAngle = 0;
	cam.focusDist = 10.0;

	cam.Render(world);
}

void Earth()
{
	auto earthTexture = make_shared<imageTexture>("image/earthmap.jpg");
	auto earthSurface = make_shared<lambertian>(earthTexture);
	auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earthSurface);


	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = point3(0, 0, 12);
	cam.lookAt = point3(0, 0, 0);
	cam.viewUp = point3(0, 1, 0);

	cam.defocusAngle = 0;
	cam.focusDist = 10.0;

	cam.Render(hittableList(globe));
}

void PerlinSpheres()
{
	hittableList world;
	auto pertext = make_shared<noiseTexture>(4);

	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
	
	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = point3(13, 2, 3);
	cam.lookAt = point3(0, 0, 0);
	cam.viewUp = point3(0, 1, 0);

	cam.defocusAngle = 0;
	cam.focusDist = 10.0;

	cam.Render(world);
}

int main(int argc, char* argv[])
{
	auto scene = argc > 1 ? std::atoi(argv[1]) : 1;
	switch (scene)
	{
	case 1: BouncingSpheres(); break;
	case 2: CheckeredSpheres(); break;
	case 3: Earth(); break;
	case 4: PerlinSpheres(); break;
	default: BouncingSpheres(); break;
	}
}