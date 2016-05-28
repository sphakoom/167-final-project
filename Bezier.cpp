#include "Bezier.h"

Bezier::Bezier(std::vector<glm::vec3> points) {
	toWorld = glm::mat4();
	lightSource = glm::vec3(-30.0f, -50.0f, 20.0f);
	this->points = points;

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * this->points.size(), &(this->points[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

void Bezier::draw() {
	
	// Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
	glm::mat4 MVP = Window::P * Window::V * this->toWorld;
	GLuint MatrixID = glGetUniformLocation(Window::trackShader, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	GLuint ModelID = glGetUniformLocation(Window::trackShader, "model");
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, &(this->toWorld[0][0]));
	
	glUniform1i(glGetUniformLocation(Window::trackShader, "isAnchor"), 0);
	glUniform1i(glGetUniformLocation(Window::trackShader, "isControl"), 0);
	glUniform1i(glGetUniformLocation(Window::trackShader, "isControlLine"), 0);

	glBindVertexArray(VAO);
	glLineWidth(3.0f);
	glDrawArrays(GL_LINE_STRIP, 0, points.size());
	glBindVertexArray(0);
	
}