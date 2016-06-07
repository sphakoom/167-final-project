#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/ext.hpp"
#include "Cube.h"
#include "OBJObject.h"
#include "Skybox.h"
#include "shader.h"
#include "City.h"
#include "Plant.h"
class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P;
	static glm::mat4 V;
	static glm::vec3 cam_pos;
	static glm::vec3 cam_look_at;
	static glm::vec3 cam_up;
	static GLint shaderProgram;
	static GLint skyboxShader;
	static GLint cityShader;
	static GLint helicopterShader;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static glm::tvec3<double> mapToSphere(glm::tvec3<double> point);
	static void idle_callback(GLFWwindow*);
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static bool doRayCast();
	static void initPlants();
};

#endif
