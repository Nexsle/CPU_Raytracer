# Raytracer
## Project Description
A CPU-based raytracer written in C++ based on *Ray Tracing in One Weekend*. The program output the result in a ppm image format. Features include:
- Simple primitives: sphere, quads
- Materials: lambertian, metal, dielectric, diffuse light, 
- Textures: solid color, Perlin noise, image texture
- Bounding volume hierarchy: accelerate ray-object intersection
- Depth of field
- Volumetric fog
- Matrix transformation
- Motion blur
## Example
![[Pasted image 20260312150202.png]]


![[Pasted image 20260312150229.png]]
## Building

### CMake
```bash   
mkdir build && cd build
cmake ..
cmake --build .
```
## Usage

Run with an optional scene number argument:

```bash
./Raytracing [scene] > output.ppm
```

| Scene       | Description                                      |
| ----------- | ------------------------------------------------ |
| 1 (default) | Bouncing spheres with random materials           |
| 2           | Checkered spheres                                |
| 3           | Earth texture mapped to a sphere                 |
| 4           | Perlin noise spheres                             |
| 5           | Coloured quads                                   |
| 6           | Simple light with emissive surfaces              |
| 7           | Cornell box with rotated boxes                   |
| 8           | Cornell box with volumetric smoke                |
| 9           | Final scene (high quality, 800px, 10000 samples) |



