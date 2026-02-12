#include "rtweekend.h"

#include "Hittable.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"
#include "Quad.h"

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
	cam.background = color(0.7, 0.8, 1.0);

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
	cam.background = color(0.7, 0.8, 1.0);

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
	cam.background = color(0.7, 0.8, 1.0);

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
	cam.background = color(0.7, 0.8, 1.0);

	cam.vFov = 20;
	cam.lookFrom = point3(13, 2, 3);
	cam.lookAt = point3(0, 0, 0);
	cam.viewUp = point3(0, 1, 0);

	cam.defocusAngle = 0;
	cam.focusDist = 10.0;

	cam.Render(world);
}

void Quads()
{
	hittableList world;

	//mat
	auto leftRed = make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto backGreen = make_shared<lambertian>(color(0.2, 1.0, 1.0));
	auto rightBlue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upperOrange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
	auto lowerTeal = make_shared<lambertian>(color(0.2, 0.8, 0.0));

	world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), leftRed));
	world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), lowerTeal));
	world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), rightBlue));
	world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upperOrange));
	world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), backGreen));


	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;
	cam.background = color(0.7, 0.8, 1);

	cam.vFov = 80;
	cam.lookFrom = point3(0, 0, 9);
	cam.lookAt = point3(0, 0, 0);
	cam.viewUp = point3(0, 1, 0);

	cam.defocusAngle = 0;
	cam.focusDist = 10.0;

	cam.Render(world);
}

void SimpleLight()
{
	hittableList world;
	
	auto pertext = make_shared<noiseTexture>(4);
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));


	auto difflight = make_shared<diffuseLight>(color(4, 4, 4));
	world.add(make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));
	world.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));

	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;
	cam.background = color(0, 0, 0);

	cam.vFov = 20;
	cam.lookFrom = point3(26, 3, 9);
	cam.lookAt = point3(0, 2, 0);
	cam.viewUp = point3(0, 1, 0);

	cam.defocusAngle = 0;
	cam.focusDist = 10.0;

	cam.Render(world);
}

void CornellBox()
{
	hittableList world;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuseLight>(color(15, 15, 15));

	world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
	world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
	world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

	shared_ptr<hittable> box1 = Box(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotateY>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	world.add(box1);

	shared_ptr<hittable> box2 = Box(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotateY>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	world.add(box2);

	camera cam;

	cam.aspectRatio = 1.0;
	cam.imageWidth = 600;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;
	cam.background = color(0, 0, 0);

	cam.vFov = 40;
	cam.lookFrom = point3(278, 278, -800);
	cam.lookAt = point3(278, 278, 0);
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
	case 5: Quads(); break;
	case 6: SimpleLight(); break;
	case 7: CornellBox(); break;
	default: BouncingSpheres(); break;
	}
}