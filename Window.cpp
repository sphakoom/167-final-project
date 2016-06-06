#include "window.h"

const char* window_title = "CSE 167 Final Project";

Skybox * skybox;
//Cube * cube;
City * city;
OBJObject* helicopter;

GLint Window::shaderProgram, Window::skyboxShader, Window::cityShader, Window::helicopterShader;

// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 Window::cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
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

void Window::initialize_objects()
{

	// Load the shader program. Similar to the .obj objects, different platforms expect a different directory for files
#ifdef _WIN32 // Windows (both 32 and 64 bit versions)
	shaderProgram = LoadShaders("../shader.vert", "../shader.frag");
	skyboxShader = LoadShaders("../skybox.vert", "../skybox.frag");
	cityShader = LoadShaders("../city.vert", "../city.frag");
	helicopterShader = LoadShaders("../reflection.vert", "../reflection.frag");
#else // Not windows
	shaderProgram = LoadShaders("shader.vert", "shader.frag");
	skyboxShader = LoadShaders("skybox.vert", "skybox.frag");
	cityShader = LoadShaders("city.vert", "city.frag");
#endif

	skybox = new Skybox();
	//cube = new Cube();
	city = new City();
	helicopter = new OBJObject("ka-50.obj", "jade");
}

void Window::clean_up()
{
	delete(skybox);
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

	city->draw(cityShader);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

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
			break;
		case GLFW_KEY_A:
			cam_pos.x -= 1.0f;
			cam_look_at.x -= 1.0f;
			break;
		case GLFW_KEY_S:
			cam_pos.z += 1.0f;
			cam_look_at.z += 1.0f;
			break;
		case GLFW_KEY_D:
			cam_pos.x += 1.0f;
			cam_look_at.x += 1.0f;
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
	double d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse

	v.x = (2.0*point.x - Window::width) / (double)Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0*point.y) / (double)Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0f;   // initially the mouse z position is set to zero, but this will change below

	d = sqrt((v.x * v.x) + (v.y * v.y));    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v);
	return v;  // return the mouse location on the surface of the trackball
}
