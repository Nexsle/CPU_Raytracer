#pragma once
#ifdef _MSC_VER
#pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

#include "rtweekend.h"
#include <cstdlib>
#include <iostream>

class rtwImage
{
public:
	rtwImage() {}

	rtwImage(const char* imageFilename)
	{
		auto filename = std::string(imageFilename);
		auto imageDir = getenv("RTW_IMAGES");

		if (imageDir && load(std::string(imageDir) + "/" + imageFilename)) return;


		// Hunt for the image file in some likely locations.
		if (imageDir && load(std::string(imageDir) + "/" + imageFilename)) return;
		if (load(filename)) return;
		if (load("images/" + filename)) return;
		if (load("../images/" + filename)) return;
		if (load("../../images/" + filename)) return;
		if (load("../../../images/" + filename)) return;
		if (load("../../../../images/" + filename)) return;
		if (load("../../../../../images/" + filename)) return;
		if (load("../../../../../../images/" + filename)) return;

		std::cerr << "ERROR: could not load image file '" << imageFilename << "'.\n";
	}

	~rtwImage()
	{
		delete[] bdata;
		STBI_FREE(fdata);
	}

	bool load(const std::string& filename)
	{
		auto n = bytesPerPixel;
		fdata = stbi_loadf(filename.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);
		if (fdata == nullptr) return false;

		bytesPerScanline = imageWidth * bytesPerPixel;
		ConvertToBytes();
		return true;
	}	

	int Width() const { return (fdata == nullptr) ? 0 : imageWidth; }
	int Height() const { return (fdata == nullptr) ? 0 : imageHeight; }

	const unsigned char* PixelData(int x, int y) const
	{
		static unsigned char magenta[] = { 255, 0, 255 };
		if (bdata == nullptr) return magenta;

		x = Clamp(x, 0, imageWidth);
		y = Clamp(y, 0, imageHeight);

		return bdata + y * bytesPerScanline + x * bytesPerPixel;
	}

private:
	const int bytesPerPixel = 3;
	float* fdata = nullptr;
	unsigned char* bdata = nullptr;
	int imageWidth = 0;
	int imageHeight = 0;
	int bytesPerScanline = 0;

	static int Clamp(int x, int low, int high)
	{
		if (x < low) return low;
		if (x < high) return x;

		return high - 1;
	}

	static unsigned char FloatToByte(float value)
	{
		if (value <= 0.0)
			return 0;
		if (1.0 <= value)
			return 255;
		return static_cast<unsigned char>(256.0 * value);
	}

	void ConvertToBytes()
	{
		int totalBytes = imageWidth * imageHeight * bytesPerPixel;
		bdata = new unsigned char[totalBytes];

		//convert from [0, 1] to [0, 255]
		auto* bptr = bdata;
		auto* fptr = fdata;

		for (auto i = 0; i < totalBytes; i++, bptr++, fptr++)
			*bptr = FloatToByte(*fptr);
	}
};

#ifdef _MSC_VER
#pragma warning (pop)
#endif