#include "rtweekend.h"

#include "Hittable.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"
#include "Quad.h"
#include "ConstantMedium.h"

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

void CornellSmoke()
{
	hittableList world;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuseLight>(color(7, 7, 7));

	world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
	world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
	world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

	shared_ptr<hittable> box1 = Box(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotateY>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));


	shared_ptr<hittable> box2 = Box(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotateY>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));


	world.add(make_shared<constantMedium>(box1, 0.01, color(0, 0, 0)));
	world.add(make_shared<constantMedium>(box2, 0.01, color(1, 1, 1)));

	camera cam;

	cam.aspectRatio = 1.0;
	cam.imageWidth = 600;
	cam.samplePerPixel = 200;
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

void FinalScene(int imageWidth, int samplePerPixel, int maxDepth)
{
	hittableList boxes1;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

	int boxPerSide = 20;

	for (int i = 0; i < boxPerSide; i++)
	{
		for (int j = 0; j < boxPerSide; j++)
		{
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = RandomDouble(1, 101);
			auto z1 = z0 + w;
			
			boxes1.add(Box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}

	hittableList world;

	world.add(make_shared<bvhNode>(boxes1));

	auto light = make_shared<diffuseLight>(color(7, 7, 7));
	world.add(make_shared<quad>(point3(123,554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

	auto center1 = point3(400, 400, 200);
	auto center2 = center1 + vec3(30, 0, 0);
	auto sphereMaterial = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	world.add(make_shared<sphere>(center1, center2, 50, sphereMaterial));

	world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	world.add(make_shared<sphere>(point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

	auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	world.add(boundary);
	world.add(make_shared<constantMedium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	world.add(make_shared<constantMedium>(boundary, 0.0001, color(1, 1, 1)));

	auto emat = make_shared<lambertian>(make_shared<imageTexture>("image/earthmap.jpg"));
	world.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
	auto pertext = make_shared<noiseTexture>(0.2);
	world.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	hittableList boxes2;
	auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
	}

	world.add(make_shared<translate>(make_shared<rotateY>(make_shared<bvhNode>(boxes2), 15), vec3(-100, 270, 395)));

	camera cam;

	cam.aspectRatio = 1.0;
	cam.imageWidth = imageWidth;
	cam.samplePerPixel = samplePerPixel;
	cam.maxDepth = maxDepth;
	cam.background = color(0, 0, 0);

	cam.vFov = 40;
	cam.lookFrom = point3(478, 278, -600);
	cam.lookAt = point3(278, 278, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocusAngle = 0;

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
	case 8: CornellSmoke(); break;
	case 9: FinalScene(800, 10000, 40); break;
	default: FinalScene(400, 250, 4); break;
	}
}