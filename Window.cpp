#include "window.h"

const char* window_title = "CSE 167 Final Project";

Skybox * skybox;
//Cube * cube;
City * city;
OBJObject* helicopter;
glm::vec3 helicopterPos;
glm::vec3 newCamPos;
glm::vec4 particleOffset;

bool waterOn = false;

Texture* texture;
GLuint TextureID;
GLuint CameraRight_worldspace_ID;
GLuint CameraUp_worldspace_ID;
GLuint ViewProjMatrixID;

const int MaxParticles = 100000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

static GLfloat* g_particule_position_size_data;
static GLubyte* g_particule_color_data;

GLuint billboard_vertex_buffer;
GLuint particles_position_buffer;
GLuint particles_color_buffer;

glm::mat4 ViewProjectionMatrix;

GLint Window::shaderProgram, Window::skyboxShader, Window::cityShader, Window::helicopterShader, Window::particleShader;

// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 Window::cam_look_at(0.0f, 0.0f, 0.0f);	// d  | helicopter is where the camera looks at
glm::vec3 Window::cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
static int ROTSCALE = 10;
int MOUSE_PRESS = 0;
int MOUSE_FOLLOW = 0;
int LIGHT_CONTROL = 0;
int CURRENT_LIGHT = 0;

glm::mat4 Window::P;
glm::mat4 Window::V;
glm::tvec3<double> lastPoint;
glm::tvec3<double> currPoint;

glm::tvec3<double> lastPos;
glm::tvec3<double> currPos;

std::vector<std::pair<glm::vec3 *,int>> selectables;
glm::vec3 * selected = NULL;

double lastTime = glfwGetTime();

void Window::initialize_objects()
{

	// Load the shader program. Similar to the .obj objects, different platforms expect a different directory for files
#ifdef _WIN32 // Windows (both 32 and 64 bit versions)
	shaderProgram = LoadShaders("../shader.vert", "../shader.frag");
	skyboxShader = LoadShaders("../skybox.vert", "../skybox.frag");
	cityShader = LoadShaders("../city.vert", "../city.frag");
	helicopterShader = LoadShaders("../reflection.vert", "../reflection.frag");
	particleShader = LoadShaders("../particle.vert", "../particle.frag");
#else // Not windows
	shaderProgram = LoadShaders("shader.vert", "shader.frag");
	skyboxShader = LoadShaders("skybox.vert", "skybox.frag");
	cityShader = LoadShaders("city.vert", "city.frag");
#endif

	skybox = new Skybox();
	//cube = new Cube();
	city = new City();
	helicopter = new OBJObject("ka-50.obj", "jade");

	glm::mat4 translateMatrix = glm::mat4(
		glm::tvec4<float>(1.0f, 0.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 1.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 0.0f, 1.0f, 0.0f),
		glm::tvec4<float>(helicopter->toWorld[3][0], helicopter->toWorld[3][1], helicopter->toWorld[3][2], 1.0f));

	glm::mat4 originMatrix = glm::mat4(
		glm::tvec4<float>(1.0f, 0.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 1.0f, 0.0f, 0.0f),
		glm::tvec4<float>(0.0f, 0.0f, 1.0f, 0.0f),
		glm::tvec4<float>(-helicopter->toWorld[3][0], -helicopter->toWorld[3][1], -helicopter->toWorld[3][2], 1.0f));

	glm::mat4 scaleMatrix = glm::mat4(
		glm::vec4(5.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 5.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 5.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	helicopter->toWorld[3] = helicopter->toWorld[3] + glm::vec4(0.5f, 0.0f, 0.0f, 0.0f);
	helicopter->toWorld = translateMatrix * scaleMatrix * originMatrix * helicopter->toWorld;

	//helicopter->toWorld[3].y += 2.0f;
	particleOffset.x = 0.7f;
	particleOffset.y = -1.0f;
	particleOffset.z = 0.0f;

	// ------------------ Particle stuff -----------------------------
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Vertex shader
	CameraRight_worldspace_ID = glGetUniformLocation(particleShader, "CameraRight_worldspace");
	CameraUp_worldspace_ID = glGetUniformLocation(particleShader, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(particleShader, "VP");

	// fragment shader
	TextureID = glGetUniformLocation(particleShader, "myTextureSampler");


	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];

	for (int i = 0; i<MaxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	texture = new Texture("Particle-Texture.ppm");

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	// ------------------ end particle stuff -----------------------------
}

void Window::clean_up()
{
	delete(skybox);
	delete(helicopter);
	delete(city);
	//delete(cube);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback(GLFWwindow* window)
{

	// ---------------- particle stuff -------------------------
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	lastTime = currentTime;


	// computeMatricesFromInputs();
	P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	// There should be a getCameraPosition() function in common/controls.cpp, 
	// but helicopter works too.
	glm::vec3 CameraPosition(glm::inverse(V)[3]);

	ViewProjectionMatrix = P * V;


	// Generate 10 new particule each millisecond,
	// but limit helicopter to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for (int i = 0; i<newparticles; i++) {
		int particleIndex;
		for (int i = LastUsedParticle; i<MaxParticles; i++) {
			if (ParticlesContainer[i].life < 0) {
				LastUsedParticle = i;
				particleIndex = i;
			}
		}

		for (int i = 0; i<LastUsedParticle; i++) {
			if (ParticlesContainer[i].life < 0) {
				LastUsedParticle = i;
				particleIndex = i;
			}
		}
		ParticlesContainer[particleIndex].life = 5.0f; // helicopter particle will live 5 seconds.
		ParticlesContainer[particleIndex].pos = glm::vec3(helicopter->getPosition() + particleOffset);

		float spread = 5.0f;
		glm::vec3 maindir = glm::vec3(0.0f, -10.0f, 0.0f);
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
			);

		ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


		// Very bad way to generate a random color
		ParticlesContainer[particleIndex].r = 0;
		ParticlesContainer[particleIndex].g = rand() % 256;
		ParticlesContainer[particleIndex].b = rand() % 256;
		ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

		ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

	}



	// Simulate all particles
	int ParticlesCount = 0;
	for (int i = 0; i<MaxParticles; i++) {
		Particle& p = ParticlesContainer[i]; // shortcut

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f) {
				// Simulate simple physics : gravity only, no collisions
				p.speed += glm::vec3(0.0f, -2.0f, 0.0f) * (float)delta * 0.2f;
				p.pos += p.speed * (float)delta;
				p.cameradistance = glm::length2(p.pos - CameraPosition);
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

				// Fill the GPU buffer
				g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
				g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
				g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

				g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

				g_particule_color_data[4 * ParticlesCount + 0] = p.r;
				g_particule_color_data[4 * ParticlesCount + 1] = p.g;
				g_particule_color_data[4 * ParticlesCount + 2] = p.b;
				g_particule_color_data[4 * ParticlesCount + 3] = p.a;

			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;

		}
	}

	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);


	//printf("%d\n",ParticlesCount);


	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but helicopter is outside the scope of helicopter tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming


	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

	// ---------------- end particle stuff -------------------------

	// Get the current (new) position
	glfwGetCursorPos(window, &currPoint.x, &currPoint.y);
	currPos = currPoint;
	currPos.y = height - currPoint.y;
	currPoint = mapToSphere(currPoint);

	if (MOUSE_PRESS) {
		// Get the direction
		glm::vec3 direction; 

		if (selected != NULL) {
			// Difference in pixels
			direction = currPos - lastPos;

			float velocity = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
			
			if (velocity > 0.001f) {
				glm::vec3 z_axis = glm::normalize(cam_pos - cam_look_at);
				glm::vec3 x_axis = glm::normalize(glm::cross(cam_up, z_axis));
				glm::vec3 y_axis = glm::cross(z_axis, x_axis);
				
				glm::vec3 xAmt = direction.x * 0.0001f * x_axis;
				glm::vec3 yAmt = direction.y * 0.0001f * y_axis;

				//track->translateSelected(selected, xAmt, yAmt);

				// Force redraw
				//track->redraw();
			}
		}

		// Camera rotation
		else {
			direction = currPoint - lastPoint;
			float velocity = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
			// Only register significant movement
			if (velocity > 0.0001f) {

				glm::vec3 rotAxis;
				rotAxis = glm::cross(lastPoint, currPoint);

				float rot_angle = atan(velocity / 2.0f) * 2.0f;

				// Create rotational matrix
				glm::mat4 rotationMatrix = glm::mat4(1.0f);
				rotationMatrix = glm::rotate(rotationMatrix, rot_angle, rotAxis);

				// Rotate the camera position and up vectors only
				glm::vec4 temp = rotationMatrix * glm::vec4(cam_pos, 1.0f);
				cam_pos = glm::vec3(temp.x, temp.y, temp.z);

				temp = rotationMatrix * glm::vec4(cam_up, 1.0f);
				cam_up = glm::vec3(temp.x, temp.y, temp.z);

				// Regenerate view matrix
				Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			}
		}
	}
	// else, just keep the same camera matrix

	lastPoint = currPoint;
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the skybox
	skybox->draw(skyboxShader, shaderProgram);

	// ------------------- Particle stuff ---------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	glUseProgram(particleShader);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	// Same as the billboards tutorial
	glUniform3f(CameraRight_worldspace_ID, V[0][0], V[1][0], V[2][0]);
	glUniform3f(CameraUp_worldspace_ID, V[0][1], V[1][1], V[2][1]);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, helicopter means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

								 // Draw the particules !
								 // helicopter draws many times a small triangle_strip (which looks like a quad).
								 // helicopter is equivalent to :
								 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
								 // but faster.
	if (waterOn) {
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 30000);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// ------------------- end particle stuff ---------------------------

	city->draw(cityShader);
	helicopterPos = glm::vec3(helicopter->getPosition());
	newCamPos = glm::vec3(helicopter->getPosition());

	printf("helicopterPos.x: %f\n", helicopterPos.x);
	printf("helicopterPos.y: %f\n", helicopterPos.y);
	printf("helicopterPos.z: %f\n", helicopterPos.z);

	newCamPos.x -= 3.0f;
	newCamPos.y += 1.0f;
	newCamPos.z -= 15.0f;

	V = glm::lookAt(newCamPos, helicopterPos, cam_up);

	// Switch back to regular shader
	glUseProgram(helicopterShader);
	helicopter->draw();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_W:
			cam_pos.z -= 1.0f;
			cam_look_at.z -= 1.0f;
			helicopter->toWorld[3].z -= 1.0f;
			break;
		case GLFW_KEY_A:
			cam_pos.x -= 1.0f;
			cam_look_at.x -= 1.0f;
			helicopter->toWorld[3].x -= 1.0f;
			break;
		case GLFW_KEY_S:
			cam_pos.z += 1.0f;
			cam_look_at.z += 1.0f;
			helicopter->toWorld[3].z += 1.0f;
			break;
		case GLFW_KEY_D:
			cam_pos.x += 1.0f;
			cam_look_at.x += 1.0f;
			helicopter->toWorld[3].x += 1.0f;
			break;
		case GLFW_KEY_UP:
			cam_pos.y += 1.0f;
			cam_look_at.y += 1.0f;
			helicopter->toWorld[3].y += 1.0f;
			break;
		case GLFW_KEY_DOWN:
			cam_pos.y -= 1.0f;
			cam_look_at.y -= 1.0f;
			helicopter->toWorld[3].y -= 1.0f;
			break;
		case GLFW_KEY_SPACE:
			waterOn = !waterOn;
			break;
		case GLFW_KEY_B:
			printf("Regenerating buildings\n");
			city->regenerate();
			break;
		default:
			printf("Invalid key press\n");
			break;
		}
	}

}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Holding left mouse button
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

		// Map mouse position to spherical location and store
		glfwGetCursorPos(window, &lastPoint.x, &lastPoint.y);
		lastPos = lastPoint;
		lastPos.y = height - lastPoint.y;


		// Selecting a point
		if (doRayCast()) {
			
		}

		// Moving the camera around
		else {
			// Map point to a spherical
			glMatrixMode(GL_MODELVIEW);
		}
		MOUSE_PRESS = 1;
	}

	// Releasing left mouse button
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		MOUSE_PRESS = 0;

		// Reset selected
		selected = NULL;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		MOUSE_FOLLOW = 1;
		glfwGetCursorPos(window, &lastPoint.x, &lastPoint.y);
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		MOUSE_FOLLOW = 0;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	 // grab the direction
	 glm::vec3 direction = glm::normalize(cam_pos - cam_look_at);
	
	// Negate for 'down' scroll
	if (yoffset < 0.0) {
		direction = -direction;
	}

	glm::mat4 translationMat = glm::mat4(1.0f);
	translationMat = glm::translate(translationMat, direction);
	glm::vec3 newPos = glm::vec3(translationMat * glm::vec4(cam_pos, 1.0f));

	// make sure we don't look past the lookAt point
	if (glm::length(newPos - cam_look_at) > 1.0f ) {
		cam_pos = newPos;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

/*
 *Performs a ray cast and returns its success or failure.
 * Code from: http://antongerdelan.net/opengl/raycasting.html
 */
bool Window::doRayCast()
{
	// Normalize the mouse coordinates
	float x = (2.0f * lastPoint.x) / width - 1.0f;
	float y = 1.0f - (2.0f * lastPoint.y) / height;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	// Direction of the ray
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(Window::P) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	// Ray in world space
	glm::vec3 ray_wor = glm::vec3(glm::inverse(Window::V) * ray_eye);
	// don't forget to normalise the vector at some point
	ray_wor = glm::normalize(ray_wor);

	// Now check for intersections
	for (int i = 0; i < selectables.size(); ++i) {
		float b = glm::dot(ray_wor, (cam_pos - *selectables[i].first));

		// (O - C) * (O - C) - r^2 
		float c = glm::dot(cam_pos - *selectables[i].first, cam_pos - *selectables[i].first) - 0.02f;

		float bSquaredMinusC = b*b - c;
		if (bSquaredMinusC < 0.0f) continue;
		else {
			selected = selectables[i].first;
			printf("SELECTED: %lf %lf %lf\n", selected->x, selected->y, selected->z);
			return true;
		}
	}
	lastPoint = mapToSphere(lastPoint);
	return false;
}

/*
// Utility routine to calculate the 3D position of a
// projected unit vector onto the xy-plane. Given any
// point on the xy-plane, we can think of it as the projection
// from a sphere down onto the plane. The inverse is what we
// are after.
// Code from http://web.cse.ohio-state.edu/~crawfis/Graphics/VirtualTrackball.html
*/
glm::tvec3<double> Window::mapToSphere(glm::tvec3<double> point)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
{
	glm::tvec3<double> v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	double d;     // helicopter is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse

	v.x = (2.0*point.x - Window::width) / (double)Window::width;   // helicopter calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0*point.y) / (double)Window::height;   // helicopter does the equivalent to the above for the mouse Y position
	v.z = 0.0f;   // initially the mouse z position is set to zero, but helicopter will change below

	d = sqrt((v.x * v.x) + (v.y * v.y));    // helicopter is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0;   // helicopter limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001 - d*d);  // helicopter calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v);
	return v;  // return the mouse location on the surface of the trackball
}
