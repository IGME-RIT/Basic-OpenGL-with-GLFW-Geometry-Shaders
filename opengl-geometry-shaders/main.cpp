/*
Title: Geometry Shaders
File Name: main.cpp
Copyright � 2016
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



#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "shader.h"
#include <iostream>



// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(int argc, char **argv)
{
	// Initializes the GLFW library
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Geometry Shaders", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//set resize callback
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	// Initializes the glew library
	glewInit();


    // Create some vertices.
	std::vector<glm::vec2> vertices;
    vertices.push_back(glm::vec2(-.5, .5));
    vertices.push_back(glm::vec2(.5, .5));
    vertices.push_back(glm::vec2(-.5, -.5));
    vertices.push_back(glm::vec2(.5, -.5));

    // Create a vertex buffer:
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Initialize the shaders using the shader files.
    // The geometry shader runs between the vertex and fragment shaders.
    // (The order they are attached in doesn't matter)
    Shader vertexShader, geometryShader, fragmentShader;
	vertexShader.InitFromFile("../Assets/vertex.glsl", GL_VERTEX_SHADER);
    geometryShader.InitFromFile("../Assets/geometry.glsl", GL_GEOMETRY_SHADER);
	fragmentShader.InitFromFile("../Assets/fragment.glsl", GL_FRAGMENT_SHADER);

    // What is a geometry shader, and what happens between the vertex and fragment steps?

    // Next is the Geometry Step. This is optional, because 


    //////////////////// Customizable (Optional)
    // VERTEX SHADER // Input: Vertices
    ////////////////// Output: Vertices

    // We know from earlier examples that we don't need a Vertex Shader.
    // It's optional, because it takes in vertices and outputs vertices, which means it can just pass data through.



    ///////////////////////// Fixed Function
    // PRIMITIVE ASSEMBLY // Input: Vertices
    /////////////////////// Output: Primitives

    // Then comes Primitive Assembly. In OpenGL, this is determined by the primitive type instruction passed in when calling draw.
    // Primitive Assembly takes in vertices, then combines them into primitives, and outputs those. (triangles, lines, or points)
    // If you have out variables in your vertex shader, these stay with the vertices in each primitive here.



    ////////////////////// Customizable (Optional)
    // GEOMETRY SHADER // Input: Primtives
    //////////////////// Output: Primitives

    // The geometry step is also optional. It takes in primitives, and it outputs primtives. Noticing a pattern here?
    // The primtive types we output don't even have to match the primitive types being output.
    // For example, you could turn the triangles from primitive assembly, back into points.
    // You would have a much harder time going the other way though, because you'd only get one point at a time.



    //////////////////// Fixed Function
    // RASTERIZATION // Input: Primtives
    ////////////////// Output: Fragments
    
    // After geometry comes Rasterization. The rasterization step takes primitives, and generates fragments.
    // Many things happen here, including face culling/clipping.
    // This is where out variables are interpolated, generating gradients.
    


    ////////////////////// Customizable (Optional)
    // FRAGMENT SHADER // Input: Fragments
    //////////////////// Output: Fragments

    // Finally the fragment step. At this point, we turn all of that rasterized fragments into beautiful pixels.
    // This stage is optional, since the gpu can just use the pixels from the rasterization output.



    // Create a shader program  using the 3 core customizable shader stages.
    GLuint shaderProgram = glCreateProgram();
	vertexShader.AttachTo(shaderProgram);
    geometryShader.AttachTo(shaderProgram);
	fragmentShader.AttachTo(shaderProgram);
	glLinkProgram(shaderProgram);



    float totalTime = 0;
	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
        // Calculate delta time.
        float dt = glfwGetTime();
        totalTime += dt;
        // Reset the timer.
        glfwSetTime(0);

        // Rotate all of the vertices over time.
        vertices[0] = glm::vec2(cos(totalTime), sin(totalTime));
        vertices[1] = glm::vec2(-cos(totalTime), -sin(totalTime));
        vertices[2] = glm::vec2(sin(totalTime), -cos(totalTime));
        vertices[3] = glm::vec2(-sin(totalTime), cos(totalTime));

		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);


        // Prepare to render our 4 vertices:
        // Bind the vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        // Buffer our newly updated data.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);
        // Prepare to use vertex attribute.
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnableVertexAttribArray(0);

		// Set the current shader program.
		glUseProgram(shaderProgram);
        
        // Draw!
        glDrawArrays(GL_POINTS, 0, 4);

		// Stop using the shader program.
		glUseProgram(0);

        // Turn off vertex attribute
        glDisableVertexAttribArray(0);

		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}

	// Free memory from shader program and individual shaders
	glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &vertexBuffer);

	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
