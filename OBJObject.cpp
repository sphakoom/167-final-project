#include "OBJObject.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
OBJObject::OBJObject()
{
	// Shouldn't be used
	toWorld = glm::mat4(1.0f);
	lightSource = glm::vec3(-30.0f, -50.0f, 20.0f);
}

OBJObject::OBJObject(const char *filepath, std::string material) 
{
	printf("Processing %s...", filepath);
	pointSize = 1.0;
	toWorld = glm::mat4(1.0f);
	lightSource = glm::vec3(-30.0f, -50.0f, 20.0f);
	this->material = material;
	parse(filepath);
	cutOff = 12.5f;

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);

	printf("Done.\n");
}

OBJObject::~OBJObject()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

std::vector<glm::vec3> OBJObject::getVertices()
{
	return this->vertices;
}

std::vector<glm::vec3> OBJObject::getNormals()
{
	return this->normals;
}

std::vector<unsigned int> OBJObject::getIndices()
{
	return this->indices;
}

float OBJObject::getPointSize()
{
	return this->pointSize;
}

glm::vec4 OBJObject::getPosition() {
	return this->toWorld[3];
}

/*
 * Parses an OBJ file.
 */
void OBJObject::parse(const char *filepath) 
{
	float minX = numeric_limits<float>::infinity(),
		minY = numeric_limits<float>::infinity(),
		minZ = numeric_limits<float>::infinity(),
		maxX = -numeric_limits<float>::infinity(),
		maxY = -numeric_limits<float>::infinity(),
		maxZ = -numeric_limits<float>::infinity();

	float x, y, z, r, g, b;
	string line;
	int debug = 0;
	ifstream inFile(filepath);

	if (inFile.fail() ) {
		printf("File not found and/or is invalid\n");
		return;
	}

	while( getline(inFile, line) ) {

		// Skip comments
		if (line[0] == '#') {
			continue;
		}
		// Process normals
		else if (line[1] == 'n') {
			istringstream ss(line);
			string type;
			ss >> type >> x >> y >> z;

			// Process vertices
			glm::vec3 norm;
			norm.x = x;
			norm.y = y;
			norm.z = z;
			normals.push_back(norm);
		}
		// Process faces
		else if (line[0] == 'f') {
			istringstream ss(line);
			string type, v1, v2, v3;
			ss >> type >> v1 >> v2 >> v3;

			int start1 = stoi(v1.substr(0, v1.find_first_of('/')));
			int start2 = stoi(v2.substr(0, v2.find_first_of('/')));
			int start3 = stoi(v3.substr(0, v3.find_first_of('/')));

			if (filepath == "godzilla.obj") {
				indices.push_back(start1 - 1);
				indices.push_back(start2 - 1);
				indices.push_back(start3 - 1);
			}
			if (filepath == "ka-50.obj") {
				indices.push_back(start1);
				indices.push_back(start2);
				indices.push_back(start3);
			}
			
		}
		// Process vertices
		else {
			istringstream ss(line);
			string type;
			ss >> type >> x >> y >> z >> r >> g >> b;

			if (x < minX)
				minX = x;
			if (x > maxX)
				maxX = x;

			if (y < minY)
				minY = y;
			if (y > maxY)
				maxY = y;

			if (z < minZ)
				minX = x;
			if (z > maxZ)
				maxZ = z;

			// Process vertices
			glm::vec3 vertex;
			vertex.x = x;
			vertex.y = y;
			vertex.z = z;

			vertices.push_back(vertex);
		}
	}

	float avgX, avgY, avgZ, dimX, dimY, dimZ;

	avgX = (maxX + minX)/2;
	avgY = (maxY + minY)/2;
	avgZ = (maxZ + minZ)/2;
	dimX = maxX - minX;
	dimY = maxY - minY;
	dimZ = maxZ - minZ;

	if (avgX == -numeric_limits<float>::infinity() || avgX == numeric_limits<float>::infinity())
		avgX = 0.0f;
	if (avgY == -numeric_limits<float>::infinity() || avgY == numeric_limits<float>::infinity())
		avgY = 0.0f;
	if (avgZ == -numeric_limits<float>::infinity() || avgZ == numeric_limits<float>::infinity()) {
		avgZ = 0.0f;
	}

	float maxDim;

	if (dimX > dimY)
		maxDim = dimX;
	else if (dimY > dimZ)
		maxDim = dimY;
	else
		maxDim = dimZ;

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i].x -= avgX;
		vertices[i].x /= maxDim;

		vertices[i].y -= avgY;
		vertices[i].y /= maxDim;

		vertices[i].z -= avgZ;
		vertices[i].z /= maxDim;
	}

	inFile.close();
}

void OBJObject::loadData()
{
}

void OBJObject::draw() {

	GLint helicopterShader = Window::helicopterShader;

	GLuint normalID = glGetUniformLocation(helicopterShader, "model");
	glUniformMatrix4fv(normalID, 1, GL_FALSE, &toWorld[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(helicopterShader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(helicopterShader, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniform3f(glGetUniformLocation(helicopterShader, "cameraPos"), Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
	glUniform1i(glGetUniformLocation(helicopterShader, "skybox"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OBJObject::draw(GLuint shaderProgram, int lightOption)
{
	// Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
	glm::mat4 MVP = Window::P * Window::V * this->toWorld;
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

	glUniform1i(glGetUniformLocation(shaderProgram, "lightMode"), lightOption);

	// Set LIGHT
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), this->lightSource[0], this->lightSource[1], this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.8f, 0.8f, 0.8f);
	

	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].position"), -this->lightSource[0], -this->lightSource[1], -this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 0.8f, 0.8f, 0.8f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.5f);
	
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), -this->lightSource[0], -this->lightSource[1], -this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), this->lightSource[0], this->lightSource[1], this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutOff"), glm::cos(glm::radians(cutOff)));
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutOff"), glm::cos(glm::radians(cutOff + 2.5f)));

	// Set MATERIAL
	if (this->material.compare("jade") == 0) {
		// Set JADE material
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 12.8f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.135f, 0.2225f, 0.1575f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.54f, 0.89f, 0.63f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.316228f, 0.316228f, 0.316228f);
	}
	// Chrome
	else if (this->material.compare("chrome") == 0) {
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 76.8f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.25f, 0.25f, 0.25f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.774597f, 0.774597f, 0.774597f);
	}
	// Copper
	else {
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 12.8f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.19125f, 0.0735f, 0.0225f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.7038f, 0.27048f, 0.0828f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.256777f, 0.137622f, 0.086014f);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OBJObject::draw(GLuint shaderProgram, int lightOption, glm::mat4 model)
{
	// Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
	glm::mat4 MVP = Window::P * Window::V * model;
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

	glUniform1i(glGetUniformLocation(shaderProgram, "lightMode"), lightOption);

	// Set LIGHT
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), this->lightSource[0], this->lightSource[1], this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.8f, 0.8f, 0.8f);


	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].position"), -this->lightSource[0], -this->lightSource[1], -this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 0.8f, 0.8f, 0.8f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.5f);

	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), -this->lightSource[0], -this->lightSource[1], -this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), this->lightSource[0], this->lightSource[1], this->lightSource[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutOff"), glm::cos(glm::radians(cutOff)));
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutOff"), glm::cos(glm::radians(cutOff + 2.5f)));

	// Set MATERIAL
	if (this->material.compare("jade") == 0) {
		// Set JADE material
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 12.8f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.135f, 0.2225f, 0.1575f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.54f, 0.89f, 0.63f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.316228f, 0.316228f, 0.316228f);
	}
	// Chrome
	else if (this->material.compare("chrome") == 0) {
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 76.8f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.25f, 0.25f, 0.25f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.774597f, 0.774597f, 0.774597f);
	}
	// Copper
	else {
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 12.8f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.19125f, 0.0735f, 0.0225f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.7038f, 0.27048f, 0.0828f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.256777f, 0.137622f, 0.086014f);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OBJObject::draw(GLuint shaderProgram, glm::vec3 camera_position)
{
	// set model, view, and projection
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &(toWorld[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &(Window::V[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &(Window::P[0][0]));

	// set camera position and skybox
	glUniform3f(glGetUniformLocation(shaderProgram, "cameraPos"), camera_position.x, camera_position.y, camera_position.z);
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OBJObject::update() 
{
	spin(1.0f);
}

/**
* Continuously spins an object.
*/
void OBJObject::spin(float deg)
{
	glm::mat4 toOriginMat = glm::mat4();
	toOriginMat[3] = glm::vec4(-this->toWorld[3][0], -this->toWorld[3][1], -this->toWorld[3][2], 1.0f);

	glm::mat4 awayOriginMat = glm::mat4();
	awayOriginMat[3] = glm::vec4(this->toWorld[3][0], this->toWorld[3][1], this->toWorld[3][2], 1.0f);

	this->toWorld = awayOriginMat * toOriginMat * this->toWorld;
	this->toWorld = glm::rotate(this->toWorld, deg, glm::vec3(0, 1, 0));
}

/**
* Changes the point size.
*/
void OBJObject::manipulatePointSize(bool decrease)
{
	if (decrease) {
		this->pointSize -= 1.0f;
	}
	else {
		this->pointSize += 1.0f;
	}

	if (this->pointSize == 1.0f)
		glPointSize(this->pointSize);
	else
		glPointSize(10*this->pointSize);
}

/**
* Changes the point size.
*/
void OBJObject::manipulateRasterizedPointSize(bool decrease)
{
	if (decrease && this->pointSize > 1.0f) {
		this->pointSize -= 1.0f;
	}
	else {
		this->pointSize += 1.0f;
	}
}

/**
* Translates the object in the XYZ space.
*/
void OBJObject::manipulateXYZ(glm::vec3 direction) 
{
	glm::tvec4<float> movement = glm::tvec4<float>(0.0f, 0.0f, 0.0f, 1.0f);

	if (direction.x > 0) {
		movement[0] = 0.05f;
	}
	else if( direction.x < 0 ) {
		movement[0] = -0.05f;
	}

	if (direction.y > 0) {
		movement[1] = -0.05f;
	}
	else if (direction.y < 0) {
		movement[1] = 0.05f;
	}

	glm::mat4 translationMat = glm::mat4(
		glm::tvec4<float>(1.0f, 0.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 1.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 0.0f, 1.0f, 0.0f),
		movement);

	this->toWorld = translationMat * this->toWorld;
}

/**
* Scales an object relative to its center
*/
void OBJObject::scale(float scaleFactor)
{
	float scale;
	scale = scaleFactor;
	
	
	// Translate object to origin, scale, and then translate back
	// Scale matrix has the scalar along the diagonal
	glm::mat4 scaleMat = glm::mat4(scale);
	scaleMat[3][3] = 1.0f;

	glm::mat4 toOriginMat = glm::mat4();
	toOriginMat[3] = glm::tvec4<float>(-this->toWorld[3][0], -this->toWorld[3][1], -this->toWorld[3][2], 1.0f);

	glm::mat4 awayOriginMat = glm::mat4();
	awayOriginMat[3] = glm::tvec4<float>(this->toWorld[3][0], this->toWorld[3][1], this->toWorld[3][2], 1.0f);

	this->toWorld = awayOriginMat * scaleMat * toOriginMat * this->toWorld;
}

/**
 * Orbits an object around the z-axis.
 */
void OBJObject::orbit(bool counter)
{
	float deg;

	if (counter)
		deg = -0.1f;
	else
		deg = 0.1f;

	glm::mat4 rotationMat = glm::mat4 (
			glm::tvec4<float>(cos(deg), sin(deg), 0.0f, 0.0f),
			glm::tvec4<float>(-sin(deg), cos(deg), 0.0f, 0.0f),
			glm::tvec4<float>(0.0f, 0.0f, 1.0f, 0.0f),
			glm::tvec4<float>(0.0f, 0.0f, 0.0f, 1.0f)
		);

	this->toWorld = rotationMat * this->toWorld;
}

void OBJObject::doTrackballRotation(glm::tvec3<double>lastPoint, glm::tvec3<double>currPoint, float velocity)
{
	glm::vec3 rotAxis;
	rotAxis = glm::cross(lastPoint, currPoint);

	float rot_angle = atan(velocity / 2.0f) * 2.0f;
	this->toWorld = glm::rotate(this->toWorld, rot_angle, rotAxis);
}

void OBJObject::doDirectionalLight(glm::tvec3<double>direction, int lightOption)
{
	if (lightOption == 1 || lightOption == 2)
		this->lightSource = -direction;
	else if (lightOption == 3)
		this->lightSource = direction;
	else {
		this->lightSource += direction;
	}
}

void OBJObject::scroll(double amt)
{
	glm::tvec4<float> direction;

	// Negative amount = scroll down = pull forward
	if (amt < 0) {
		direction = glm::tvec4<float>(0.0f, 0.0f, 1.0f, 1.0f);
	}
	// Positive amount = scroll up = push back
	else {
		direction = glm::tvec4<float>(0.0f, 0.0f, -1.0f, 1.0f);
	}

	glm::mat4 translationMat = glm::mat4(
		glm::tvec4<float>(1.0f, 0.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 1.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 0.0f, 1.0f, 0.0f),
		direction);

	this->toWorld = translationMat * this->toWorld;
}

void OBJObject::changeLightDistance(double amt, int lightOption)
{
	// Only change distance for point + spot lights
	if (lightOption == 2 || lightOption == 3) {
		// Negative amount = scroll down = pull forward
		if (amt < 0) {
			this->lightSource.z += 1.0f;
		}
		// Positive amount = scroll up = push back
		else {
			this->lightSource.z -= 1.0f;
		}
	}
}

/**
* Resets all transformations on the object.
*/
void OBJObject::reset() 
{
	toWorld = glm::mat4(1.0f);
	this->pointSize = 1.0f;
	glPointSize(this->pointSize);
}

void OBJObject::setInitial(glm::vec3 position) {
	this->toWorld[3] = glm::vec4(position, 1.0f);
}

void OBJObject::update(glm::vec3 newPosition) {
	/*
	glm::vec3 oldPos = glm::vec3(this->toWorld[3]);
	glm::vec3 new_z = glm::normalize(newPosition - oldPos);
	glm::vec3 new_x = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), new_z));
	glm::vec3 new_y = glm::normalize(glm::cross(new_z, new_x));

	glm::mat4 proj = glm::mat4(
		glm::vec4(new_x, 0.0f),
		glm::vec4(new_y, 0.0f),
		glm::vec4(new_z, 0.0f),
		glm::vec4(oldPos, 1.0f)
		);

	glm::vec4 newPos = proj * glm::vec4(newPosition, 1.0f);

	this->toWorld = proj * this->toWorld;
	
	this->toWorld[3] = proj * glm::vec4(newPosition, 1.0f);
	*/
	this->toWorld[3] = glm::vec4(newPosition, 1.0f);
}