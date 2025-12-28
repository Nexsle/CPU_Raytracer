#include "rtweekend.h"

#include "Hittable.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"

int main() 
{
	hittableList world;

	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplePerPixel = 100;
	cam.maxDepth = 50;

	cam.Render(world);
}