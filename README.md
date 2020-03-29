# Ray-Tracing

This program is to implement simple ray tracing algorithm. It takes an input file which obtains information about scene and produces ppm file according to 
input file. Input file should be given as command line argument. If you run this program on VS, then you can give command line argument as follows:
   - `Project Properties` -> `Debugging` -> `Command Arguments` -> `test1.in`

#### Input File
- `name_of_output_file.ppm`
- `width(int) height(int)`
- `(float) (float) (float) //camera's eye`
- `(float) (float) (float) //camera's up`
- `(float) (float) (float) //camera's at`
- `(float) //fovy`
- `number_of_light_sources` ,then each successive line about light sources contains
- ` x y z r g b a b c (all floats where (x,y,z) coordinate of light source,(r,g,b) RGB components of light source intensity and (a,b,c) of attenuation formula`
- `number_of_pigments`, then each successive line about pigments contains
- `solid(keyword) r g b (floats)` // defines color of surface. For now, program only supports solid colors.
- `number_of_surfaces` then each successive line about surfaces contains
- `ka(ambient coefficient) kd(diffuse coefficient) ks(specular coefficient) α(shineness) kr(reflectivity coefficient)`
- `number_of_objects` then each successive line about objects contains
- `sphere(keyword) x y z r // (x,y,z) center coordinates of object and r is radius` For now, program only supports spheres.

#### Output File
Output will be in the format of `PPM P6` and can be viewed by `GIMP`
#### Dependencies
- Program should run in `win32` environment.

#### Yaşar Can Kakdaş
