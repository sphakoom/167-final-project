#include "Road.h"

Road::Road(int width, int depth)
{
	srand(time(NULL));

	int maxWidth = width / 2;
	int maxDepth = depth / 2;

	glm::vec3 roadNode = glm::vec3();

	roadNode.x = ((float)rand() / RAND_MAX) * width - maxWidth;
	roadNode.z = ((float)rand() / RAND_MAX) * depth - maxDepth;

	vertices.push_back(roadNode);

	glm::vec3 roadNode2 = glm::vec3();
	roadNode2.x = ((float)rand() / RAND_MAX) * width - maxWidth;
	roadNode2.z = ((float)rand() / RAND_MAX) * depth - maxDepth;

	vertices.push_back(roadNode2);

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0);
}

void Road::draw(GLint shaderProgram)
{
	glm::mat4 MVP = Window::P * Window::V * toWorld;
	// We need to calculate this because as of GLSL version 1.40 (OpenGL 3.1, released March 2009), gl_ModelViewProjectionMatrix has been
	// removed from the language. The user is expected to supply this matrix to the shader when using modern OpenGL.
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	
	glLineWidth(10.0f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glBindVertexArray(0);
}