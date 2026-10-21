#pragma once
#include "common.h"
#include "vectorMath.h"



typedef struct {
	double x, y;
	map<int, bool>mouseButtonState;

}mouseState_t;


class EventManager {

public:
	inline static map<int, bool>keyState;
	inline static mouseState_t mouseState;

	static void init(GLFWwindow* window);
	static void keybEventManager(GLFWwindow* window, int key, int scancode, int action, int
		mods);
	static void mousePosEvent(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonEvent(GLFWwindow* window, int button, int action, int mods);

};

