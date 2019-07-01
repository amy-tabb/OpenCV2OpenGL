# OpenCV2OpenGL

Minimal working examples for converting OpenCV cameras to OpenGL cameras, render, and save as Mat images in OpenCV.

These examples have a MIT license and are created by Amy Tabb as a companion to a tutorial series, at [amytabb.com](https://amytabb.com/ts/2019_06_28/).

There are no restrictions in your use of this code.  However, no guarantees are expressed or implied.


## Dependencies

This code uses >= OpenCV 3.0, Eigen, OpenGL, glad, glfw3, glm, and has been tested on Ubuntu 16.04 and Ubuntu 18.04.  

1. [OpenCV](https://opencv.org/) While OpenCV is available from distribution repositories, my long experience with it is has always been to build from the source (from [Github](https://github.com/opencv/opencv)) to get the best results.  **I have used OpenCV 4.x, which requires a >= C++11 compiler.  I enable the GNU version with a flag : `-std=gnu++11.`  There other ways of doing this, as well as other choices of compiler.** OpenCV 3.x should work fine (but has not been tested ...)  Required libraries are: `opencv_core, opencv_highgui, opencv_imgproc, opencv_imgcodecs` . 

3.  Check the include directory for [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), Eigen is a header-only library. 

4. Installation of glfw3 and glad are covered at [this page](https://learnopengl.com/Getting-started/Creating-a-window) of LearnOpenGL.  For glad, files are downloaded according to your specifications.  The files I have included are for OpenGL 3.3, which is old enough to cover many systems.  The files for Example1 are in Example1->include, so you can either try to use the ones provided, or download your own, and specify the include path.

5. GLM is another header-only library, and can be found [here](https://glm.g-truc.net/0.9.9/index.html), I tend to install everything from the source (versus the distribution repositories), but libglm-dev is also avaliable there from Ubuntu for a no-hassle installation.

6. Linking: link with these libraries: -lglfw3 -lGLU -lglut -lm -lGL -lpthread -ldl -lrt -lX11 -lXrandr -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui

7. At the moment there is only Example, but these are Eclipse IDE projects. If you use Eclipse, you can import the project and alter the include paths from there.

## Running

For help, run with --help.

Otherwise, each example will take different arguments and this repository will change over time.  Refer to the website for samples or run --help to find out arguments for each example.

## Model source

The horse model is from the [Georgia Tech Large Model Archive](https://www.cc.gatech.edu/projects/large_models/).  The number of triangles was reduced, and color was introduced by me, by playing around in Meshlab.

## 3D model viewer

There are many 3D model viewers out there, I use [Meshlab](http://www.meshlab.net/), which is free.



