/*
Title: Geometry Shaders
File Name: geometry.glsl
Copyright ? 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#version 400 core

// Defines that we will receive primitives of the type POINTS in this geometry shader from the vertex shader.
layout(points) in;	
// Under the hood, this defines our input gl_in is an array defined like this:
/*
in gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
} gl_in[];
*/
// The size of the array is determined by the primitive type.
// [1] for points, [2] for lines, and [3] for triangles.

// Defines that we will output primitives of the type TRIANGLE_STRIP to the rasterizer.
// line_strip, and points are also options for the output type. (try them out)
// Also, declares a maximum number of vertices to be output. (this is required, as it defaults to 0)
// If the number of vertices output exceeds this value, they will be ignored.
layout(triangle_strip, max_vertices = 4) out;

// Defines how many times to run the geometry shader for each input.
// This is effectively the same thing as putting a for loop in main.
// gl_InvocationID can be used as the index.
layout(invocations = 8) in;

out vec4 color;

void main()
{
	// Geometry will increase in size for each invocation.
	float scale = pow(.75f, 7 - gl_InvocationID);

	// Pick a color. gl_PrimitiveIDIn is the index of the vertex being rendered in the vertex buffer.
	color = vec4(gl_PrimitiveIDIn % 2, gl_PrimitiveIDIn / 2, 1, 1);

	// Make color scale with each invocation.
	color.xyz *= (gl_InvocationID + 1) / 8.f;

	// Here we get the position from gl_in. Since we're using points, we can only read from gl_in[0]
	vec4 pos = gl_in[0].gl_Position;

		
	// Emit vertex adds whatever is currently stored in gl_Position, and any other out values.
	// In this example, each vertex output by the geometry shader has position and color.
	// Color is set once above and used for all 4 vertices.
	gl_Position = pos + vec4(-.25f, .25f, 0, 0);
	gl_Position.xy *= scale;
	EmitVertex();

	// Do other 3 vertices for a quad
	gl_Position = pos + vec4(.25f, .25f, 0, 0);
	gl_Position.xy *= scale;
	EmitVertex();

	gl_Position = pos + vec4(-.25f, -.25f, 0, 0);
	gl_Position.xy *= scale;
	EmitVertex();

	gl_Position = pos + vec4(.25f, -.25f, 0, 0);
	gl_Position.xy *= scale;
	EmitVertex();


	// EndPrimitive tells the program to output a triangle strip from the previously emitted vertices, and start working on a new one.
	// This is called multiple times in this example to create several rectangles.
	EndPrimitive();
}