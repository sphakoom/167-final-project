#include "Cylinder.h"

Cylinder::Cylinder(const char * filename) : OBJObject(filename, "jade") {
	toWorld = glm::mat4(1.0f);

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

									  // Bind normals
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * normals.size(), &(normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);

	// Bind indices
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	printf("Done.\n");
}

void Cylinder::draw() {
	// Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
	glm::mat4 MVP = Window::P * Window::V * this->toWorld;
	GLint shaderProgram = Window::shaderProgram;
	// We need to calculate this because as of GLSL version 1.40 (OpenGL 3.1, released March 2009), gl_ModelViewProjectionMatrix has been
	// removed from the language. The user is expected to supply this matrix to the shader when using modern OpenGL.
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	GLuint ModelID = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, &(this->toWorld[0][0]));

	glm::mat4 C_inverse = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 20.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
		);

	GLuint ViewID = glGetUniformLocation(shaderProgram, "viewPos");
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &(C_inverse[0][0]));

	// Set LIGHT
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), this->lightSource[0], this->lightSource[1], this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.8f, 0.8f, 0.8f);

	// Set JADE material
	glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 12.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.135f, 0.2225f, 0.1575f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.54f, 0.89f, 0.63f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.316228f, 0.316228f, 0.316228f);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Cylinder::update(glm::mat4 C) {

}
