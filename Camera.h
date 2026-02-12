#pragma once
#include "Hittable.h"
#include "Material.h"

class camera
{
public:
	double aspectRatio = 1.0;
	int imageWidth = 100;
	int samplePerPixel = 10; //num of random sample per pixel
	int maxDepth = 10; //max num of ray bounces
	color background;

	double vFov = 90; //vertical view angle
	point3 lookFrom = point3(0, 0, 0);
	point3 lookAt = point3(0, 0, -1);
	vec3 viewUp = vec3(0, 1, 0);

	double defocusAngle = 0;
	double focusDist = 10; //distance from cam to perfect focus point

	void Render(const hittable& world)
	{
		Initialize();

		std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++)
		{
			std::clog << "\rScanline remaining: " << (imageHeight - j) << ' ' << std::flush;
			for (int i = 0; i < imageWidth; i++)
			{
				color pixelColor(0, 0, 0);
				for (int sample = 0; sample < samplePerPixel; sample++)
				{
					ray r = GetRay(i, j);
					pixelColor += RayColor(r,maxDepth, world);
				}

				WriteColor(std::cout, pixelColor * pixelSampleScale);
			}
		}

		std::clog << "\rDone.               \n";
	}

private:
	int imageHeight;
	point3 center; //cam center
	point3 pixel00Loc;
	vec3 pixelDeltaU;
	vec3 pixelDeltaV;
	double pixelSampleScale;
	vec3 u, v, w; //camera frame basis vectors
	vec3 defocusDiskU;
	vec3 defocusDiskV;


	void Initialize()
	{
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = imageHeight < 1 ? 1 : imageHeight;

		pixelSampleScale = 1.0 / samplePerPixel;  

		center = lookFrom;

		//Viewport
		auto theta = DegreeToRadians(vFov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2* h * focusDist;
		auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(viewUp, w));
		v = Cross(w, u);

		auto viewportU = viewportWidth * u;
		auto viewportV = viewportHeight * -v;
		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		//location of the upper left pixel
		auto viewportUpperLeft = center - (focusDist * w) - viewportU/2 - viewportV/2;
		pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

		auto defocusRadius = focusDist * std::tan(DegreeToRadians(defocusAngle / 2));
		defocusDiskU = u * defocusRadius;
		defocusDiskV = v * defocusRadius;
	}

	color RayColor(const ray& r,int depth, const hittable& world)const
	{
		if (depth <= 0)
			return color(0, 0, 0);

		hitRecord rec;
		if (!world.hit(r, interval(0.001, +infinity), rec))
			return background;

		ray scattered;
		color attenuation;
		color colorFromEmmission = rec.mat->Emmited(rec.u, rec.v, rec.p);

		if (!rec.mat->scatter(r, rec, attenuation, scattered))
		{
			return colorFromEmmission;
		}

		color colorFromScatter = attenuation * RayColor(scattered, depth - 1, world);

		return colorFromEmmission + colorFromScatter;
	}

	ray GetRay(int i, int j) const
	{
		//create a ray originating from the defocus disk and direct at a ramdom point around the pixel i, j

		auto offset = SampleSquare();
		auto pixelSample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

		auto rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();
		auto rayDirection = pixelSample - rayOrigin;
		auto rayTime = RandomDouble();


		return ray(rayOrigin, rayDirection, rayTime);
	}

	vec3 SampleSquare() const
	{
		return vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
	}

	point3 DefocusDiskSample() const
	{
		auto p = RandomInUnitDisk();
		return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
	}
};