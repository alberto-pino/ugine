//#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//#endif


#include "engine.h"
#include "../lib/glfw3/glfw3.h"

#include <iostream>

#define FULLSCREEN false


int main(int, char**) {
	if ( glfwInit() != GLFW_TRUE ) {
		std::cout << "could not initalize glfw" << std::endl;
		return -1;
	}
	atexit(glfwTerminate);


	// create window
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(1200, 800, "U-gine", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window) {
		std::cout << "could not create glfw window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);



	// init engine


	if ( !init() ) {
		std::cout << "could not initialize engine" << std::endl;
		return -1;
	}
	// create models
	std::shared_ptr<Mesh> skyboxMesh = Mesh::load("data/skybox.msh.xml");
	std::shared_ptr<Model> skyboxModel = std::make_shared<Model>(skyboxMesh);
	skyboxModel->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
 
	std::shared_ptr<Mesh> monkeyMesh = Mesh::load("data/suzanne_refract.msh.xml");
	std::shared_ptr<Model> monkeyModel = std::make_shared<Model>(monkeyMesh);
	monkeyModel->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	monkeyModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	std::shared_ptr<Mesh> cubeMesh = Mesh::load("data/cube.msh.xml");
	std::shared_ptr<Model> cubeModel = std::make_shared<Model>(cubeMesh);
	cubeModel->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	cubeModel->setPosition(glm::vec3(-4.0f, 0.0f, 2.0f));

	std::shared_ptr<Mesh> teapotMesh = Mesh::load("data/teapot_reflect.msh.xml");
	std::shared_ptr<Model> teapotModel = std::make_shared<Model>(teapotMesh);
	teapotModel->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	teapotModel->setPosition(glm::vec3(4.0f, 0.0f, 2.0f));


	// create world
	World world;

	world.addEntity(skyboxModel);
	world.addEntity(monkeyModel);
	world.addEntity(cubeModel);
	world.addEntity(teapotModel);


	// add lights
	world.setAmbient(glm::vec3(0.1, 0.1, 0.1));

	std::shared_ptr<Light> pointLight = std::make_shared<Light>(glm::vec3(0.0f, 0.0f, 0.0f), Light::Type::POINT
																, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	pointLight->setEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	world.addEntity(pointLight);


	// create camera
	std::shared_ptr<Camera> cam = std::make_shared<Camera>();
	cam->setEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	cam->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cam->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));

	world.addEntity(cam);

	double x0, y0, x, y;
	float angle= 0.0f;

	glfwGetCursorPos(window, &x0, &y0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	float lastTime = static_cast<float>(glfwGetTime());

	//std::cout << lastTime<< std::endl;

	while ( !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
		// update delta time
		float newTime = static_cast<float>(glfwGetTime());

		float deltaTime = newTime - lastTime;
		lastTime = newTime;

		// WASD
		/*
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		if (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP) || state == GLFW_PRESS)
		{
			cam->move(glm::vec3(0.0f, 0.0f, -1) * deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN))
		{
			cam->move(glm::vec3(0.0f, 0.0f, 1) * deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT))
		{
			cam->move(glm::vec3(-1, 0.0f, 0.0f) * deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			cam->move(glm::vec3(1, 0.0f, 0.0f) * deltaTime);
		}

		//mouse
		glfwGetCursorPos(window, &x, &y);

		glm::vec3 newRotation = glm::vec3((y0 - y), (x0 - x), 0.0f) * 0.6f;
		glm::vec3 currentRot = cam->getEuler();
		cam->setEuler((currentRot + newRotation));
		y0 = y;
		x0 = x;
		*/



		// get updated screen size
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);

		// update camera projection & viewport
		cam->setProjection(glm::perspective<float>(glm::radians(60.0f), static_cast<float>(screenWidth) / screenHeight, 1, 100));
		cam->setViewport(glm::ivec4(0, 0, screenWidth, screenHeight));

		// Set projection matrix in case the screen has been resized
		glm::mat4 projectionMatrix = glm::perspective(45.0f,
			static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
		cam->setProjection(projectionMatrix);
		//std::cout << "prueba" << std::endl;

		//camera, light and skybox rotation
		
		cam->setPosition(glm::vec3(0, 0, 0));
		angle += 6.0f * deltaTime;
		cam->setEuler(glm::vec3(-10, angle, 0));
		cam->move(glm::vec3(0, 0, 9));
		cam->setPosition(glm::vec3(0, 0, 0) + cam->getPosition());

		pointLight->setPosition(glm::vec3(0, 0, 0) + cam->getPosition());

		skyboxModel->setPosition(glm::vec3(0, 0, 0) + cam->getPosition());
		
		// update & draw world 
		world.update(deltaTime);
		world.draw();

		// update swap chain & process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}