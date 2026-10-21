#include "EventManager.h"

void EventManager::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keybEventManager);
	glfwSetMouseButtonCallback(window, mouseButtonEvent); //Para recibir iteraciones de los botones
	glfwSetCursorPosCallback(window, mousePosEvent); //Para cada vez que se mueva

	//Atrapar el raton en la ventana
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Cambiar la posicion del cursor al 0,0
	glfwSetCursorPos(window, 0, 0);


}

void EventManager::keybEventManager(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action) {
	case GLFW_PRESS:
		keyState[key] = true;
		break;
	case GLFW_REPEAT:
		break;
	case GLFW_RELEASE:
		keyState[key] = false;
		break;
	}
}

void EventManager::mousePosEvent(GLFWwindow* window, double xpos, double ypos)
{
	//Tenemos que compararlo con su posicion anterior, para saber que se ha movido.

	if (mouseState.x != xpos or mouseState.y != ypos) {
		mouseState.x = xpos;
		mouseState.y = ypos;

		cout << "x " << mouseState.x << " y " << mouseState.y << endl;
	}
}

void EventManager::mouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{

	switch (action) {
	case GLFW_PRESS:
		mouseState.mouseButtonState[button] = true;
		break;
	case GLFW_REPEAT:
		break;
	case GLFW_RELEASE:
		mouseState.mouseButtonState[button] = false;
		break;
	}
}
