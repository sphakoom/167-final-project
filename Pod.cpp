#include "Pod.h"
#include "Window.h"

Pod::Pod(const char * filename) : OBJObject(filename, "jade") {
	
}

void Pod::draw() {
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
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Pod::update(glm::mat4 C) {
	this->toWorld = C * this->toWorld;
}