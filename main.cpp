#include "rtweekend.h"

#include "Hittable.h"
#include "Hittable_List.h"
#include "Sphere.h"


color RayColor(const ray& r, const hittable& world)
{
	hitRecord rec;
	if (world.hit(r, interval(0, +infinity), rec))
		return 0.5 * (rec.normal + color(1, 1, 1)); //visuallise normals as color

	//Sky gradient if no hit
	vec3 unitDirection = UnitVector(r.Direction());
	auto a = 0.5 * (unitDirection.y() + 1.0f);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}



int main() 
{
	// Image
	auto aspectRatio = 16.0 / 9.0;
	int imageWidth = 400;
	int imageHeight = int(imageWidth / aspectRatio);
	imageHeight = imageHeight < 1 ? 1 : imageHeight;


	//World
	hittableList world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


	//Camera
	auto focalLength = 1.0; //distance between camera and viewport
	auto viewportHeight = 2.0;
	auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
	auto cameraCenter = point3(0, 0, 0);

	auto viewportU = vec3(viewportWidth, 0, 0);
	auto viewportV = vec3(0, -viewportHeight, 0);
	auto pixelDeltaU = viewportU / imageWidth;
	auto pixelDeltaV = viewportV / imageHeight;

	//location of the upper left pixel
	auto viewportUpperLeft = cameraCenter - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
	auto pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);


	//Render
	std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	for (int i = 0; i < imageHeight; i++)
	{
		std::clog << "\rScanline remaining: " << (imageHeight - i) << ' ' << std::flush;
		for (int j = 0; j < imageWidth; j++)
		{
			auto pixelCenter = pixel00Loc + (i * pixelDeltaV) + (j * pixelDeltaU);
			auto rayDirection = pixelCenter - cameraCenter;
			ray r(cameraCenter, rayDirection);

			color pixelColor = RayColor(r, world);
			WriteColor(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone.               \n";
}