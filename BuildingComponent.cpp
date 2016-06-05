#include "BuildingComponent.h"

const GLuint rectangleIndices[] = {  // Note that we start from 0!
	// Front face
	0, 1, 2,
	2, 3, 0,
	// Top face
	1, 5, 6,
	6, 2, 1,
	// Back face
	7, 6, 5,
	5, 4, 7,
	// Bottom face
	4, 0, 3,
	3, 7, 4,
	// Left face
	4, 5, 1,
	1, 0, 4,
	// Right face
	3, 2, 6,
	6, 7, 3
};

const GLuint triangleIndices[] = {
	// Front
	0, 1, 4,
	// Back
	3, 2, 4,
	// Left
	3, 0, 4,
	// Right
	2, 4, 1,
	// Bottom
	0, 1, 2, 
	2, 3, 0,
};


BuildingComponent::BuildingComponent(float height, float width, glm::vec3 origin, int type)
{
	toWorld = glm::mat4(1.0f);
	componentType = type;

	float halfWidth = width / 2.0f;

	switch (type) {
		case RECTANGLE:
			// FRONT vertices
			vertices.push_back(glm::vec3(-halfWidth + origin.x, origin.y, halfWidth));	// bottom left
			vertices.push_back(glm::vec3(halfWidth + origin.x, origin.y, halfWidth));	// bottom right
			vertices.push_back(glm::vec3(halfWidth + origin.x, origin.y + height, halfWidth));	// top right
			vertices.push_back(glm::vec3(-halfWidth + origin.x, origin.y + height, halfWidth));	// top left

			// Back vertices
			vertices.push_back(glm::vec3(-halfWidth + origin.x, origin.y, -halfWidth));	// bottom left
			vertices.push_back(glm::vec3(halfWidth + origin.x, origin.y, -halfWidth));	// bottom right
			vertices.push_back(glm::vec3(halfWidth + origin.x, height + origin.y, -halfWidth));	// top right
			vertices.push_back(glm::vec3(-halfWidth + origin.x, height + origin.y, -halfWidth));	// top left
			break;
		case SEMI_CIRCLE:
			break;
		case PYRAMID:
			// Base vertices
			vertices.push_back(glm::vec3(-halfWidth + origin.x, origin.y, halfWidth)); // front left
			vertices.push_back(glm::vec3(halfWidth + origin.x, origin.y, halfWidth));	// front right
			vertices.push_back(glm::vec3(halfWidth + origin.x, origin.y, -halfWidth)); // back right
			vertices.push_back(glm::vec3(-halfWidth + origin.x, origin.y, -halfWidth));	// back left
			// Top vertex
			vertices.push_back(glm::vec3(origin.x, origin.y + 1.0f, 0.0f));
			break;
		default:
			break;
	}
	

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


	switch (type) {
		case RECTANGLE:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
			break;
		case SEMI_CIRCLE:
			break;
		case PYRAMID:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
			break;
			break;
		default:
			break;
	}
	


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

void BuildingComponent::draw(GLuint shaderProgram)
{
	glm::mat4 MVP = Window::P * Window::V * toWorld;
	// We need to calculate this because as of GLSL version 1.40 (OpenGL 3.1, released March 2009), gl_ModelViewProjectionMatrix has been
	// removed from the language. The user is expected to supply this matrix to the shader when using modern OpenGL.
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glBindVertexArray(VAO);

	switch (componentType) {
	case RECTANGLE:
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	case PYRAMID:
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	default:
		break;
	}

	glBindVertexArray(0);
}