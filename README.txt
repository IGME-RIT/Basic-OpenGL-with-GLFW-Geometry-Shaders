Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Geometry Shader Tutorial!
Prerequesite: Materials

Disclaimer:
Geometry Shaders are not intended for wide-use.
There are many things that Geometry Shaders "can"
be used for, but not a lot of things that they "should"
be used for. When in doubt, if you can get away with
Instancing (tutorial available), or Tesselation Shaders
(not posted yet), then please do so. This tutorial is here
so that you know how to use GS, if you need to use GS.

What is a Geometry Shader?
The Geometry Shader is a pipeline stage that
happens after the Vertex Shader, and before the Rasterizer.
It takes geometry from the Vertex Shader as input, and then outputs
different geometry to the rasterizer as output, and then the Rasterizer
sends an image to the Fragment Shader, as usual. Geometry Shaders
can also change the topology of the input; it can take in points and
turn them into triangles, or it can turn lines into cubes, etc.

Why should I not use a Geometry Shader?
Sometimes people pass points to a geometry shader, and then 
convert each point to a 3D model, so that the geometry of the model
only needs to be passed to the GPU once, and then drawn several times.
However, this is a bad idea because Instanced Rendering can do the same thing,
except faster and easier.

When should I use a Geometry Shader?
Some people recommend never using Geometry Shaders, but some people 
say they are good for rendering furry effects on animals.

How does the Geometry Shader work in this tutorial?
This shader takes one point as input, and turns that point into a
square. You can see points are taken as input:
	layout(points) in;	
Under the hood, that one line is equivalent to this:
	in gl_PerVertex
	{
		vec4 gl_Position;
		float gl_PointSize;
		float gl_ClipDistance[];
	} gl_in[];
Something important to know, it is IMPOSSIBLE to change this gl_in[] structure,
all we can do is change: 
	layout(points) in;	
to take lines or triangles as input, we CANNOT change how they are taken as input
We then set our output, which is in the form of triangles, with 4 vertices:
	layout(triangle_strip, max_vertices = 4) out;
This should allow us to generate a quad for each point we take as input.
The command EmitVertex() makes a new Vertex that is passed as output.
You can see in this shader, that EmitVertex is called four times, one for each vertex,
and the position of each vertex is moved from the center, to a corner, like so:
	gl_Position = pos + vec4(-.25f, .25f, 0, 0);
	gl_Position.xy *= scale;
	
That's everything you need to know

How to improve:
Use GPU Instancing for duplicate geometry
Use Tesselation shaders for surface manipulation
