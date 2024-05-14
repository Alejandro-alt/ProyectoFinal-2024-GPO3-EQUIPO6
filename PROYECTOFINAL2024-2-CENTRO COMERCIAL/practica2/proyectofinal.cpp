#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(12.0f, 6.0f, 61.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float movCamera = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
float tiempo; // Variable para controlar el tiempo 

//variable controlar
int currentLightIndex = 0;

//animaciones
float rot = 0.0f;
float rot1 = 0.0f;
float rot2 = 0.0f;
float rot3 = 0.0f;

//vidrio deslizante
float des = 0.0f;
float des1 = 0.0f;
bool anim4 = false;

//animacion perro
float CaballoTorso;
float CaballoTorso_offset;
float CaballoPatasT;
float CaballoPatasT_offset;
float CaballoPatasD;
float CaballoPatasD_offset;
float CaballoCabeza;
float CaballoCabeza_offset;

//camara
bool anim = false;
bool anim1 = false;
bool anim2 = true;
bool anim3 = false;

static bool direccion = true;

//Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(9.12f, 13.335f, 13.533f),
	glm::vec3(-20.446f, 6.227f, 10.998f),
	glm::vec3(-5.876f, 6.227f, 10.998f),
	glm::vec3(-5.876f, 6.227f, 19.241f)
};


//glm::vec3 pointLightPositions[] = {
//	glm::vec3(0.0f,0.0f, 0.0f),
//	glm::vec3(0.0f,0.0f, 0.0f),
//	glm::vec3(0.0f,0.0f, 0.0f),
//	glm::vec3(0.0f,0.0f, 0.0f)
//};

float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);




// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Iluminacion 2", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
		// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");

	// CARGA DE MODELOS CENTRO COMERCIAL FACHADA----------------------------------------------------------

	Model centrofachada((char*)"Models/centrofachada/centrofachada.obj");

	//// CARGA DE MODELOS CENTRO COMERCIAL FACHADA----------------------------------------------------------

	Model interior((char*)"Models/cafe/interior.obj");
	Model interiorvidrio((char*)"Models/cafe/interiorvidrio.obj");
	Model exterior((char*)"Models/cafe/exterior.obj");
	Model hoja((char*)"Models/plantas/hoja.obj");
	Model hojacom((char*)"Models/plantas/hojacom.obj");
	Model hojatallo((char*)"Models/plantas/hojatallo.obj");




	////-------------------------------------

	Model cine((char*)"Models/cine/cine.obj");
	Model sala((char*)"Models/cine/sala.obj");
	Model sala1((char*)"Models/cine/sala1.obj");
	Model anicarro((char*)"Models/anicarro/anicarro.obj");
	Model anicarro2((char*)"Models/anicarro2/anicarro2.obj");


	// CARGA DE MODELOS JUGUETERIA----------------------------------------------------------
	Model Piso((char*)"Models/Piso/Piso.obj");
	Model fachada((char*)"Models/fachada/fachada.obj");//toda la fachada
	Model loquer((char*)"Models/loquer/loquer.obj");//loquer de entrada
	Model mueble((char*)"Models/mueble/mueble.obj");// mueble de izquierda
	Model mueble2((char*)"Models/mueble/mueble2.obj");// mueble de fondo
	Model mueble3((char*)"Models/mueble/mueble3.obj");// mueble de derecha
	Model sillon((char*)"Models/sillon/sillon.obj");//
	Model mesa((char*)"Models/mesa/mesa.obj");// mesa de entrada
	Model mesa3((char*)"Models/mesa3/mesa3.obj");// mesa de entrada
	Model puerta((char*)"Models/puerta/puerta.obj");//agregado
	Model puerta2((char*)"Models/puerta/puerta2.obj");//agregado
	Model puertac1((char*)"Models/puerta/puertac1.obj");//agregado
	Model puertac2((char*)"Models/puerta/puertac2.obj");//agregado
	Model puertaarriba((char*)"Models/puerta/puertaarriba.obj");//agregado
	Model vidriod((char*)"Models/vidrio/vidriod.obj");//agregado
	Model vidrioi((char*)"Models/vidrio/vidrioi.obj");//agregado
	Model carro3VIDRIO((char*)"Models/carro3/carro3VIDRIO.obj");//agregado 
	Model escaleras((char*)"Models/escaleras/escaleras.obj");//agregado 


	//////7objetos---------
    Model carro5lego((char*)"Models/carro5lego/carro5lego.obj");// carro colores


	Model oso((char*)"Models/oso/oso.obj");// oso rojo
	Model oso2((char*)"Models/oso/oso3.obj");// oso azul
	Model osos((char*)"Models/osos/osos.obj");// oso azul

	Model balon((char*)"Models/balon/balon.obj");// 3 balones de basquet
	Model balonitalia((char*)"Models/balonitalia/balonitalia.obj");// 3 balones de basquet
	Model pelotafut((char*)"Models/pelotafut/pelotafut.obj");// 3 balones de basquet
	Model Brazuka((char*)"Models/brazuk/Brazuka.obj");// 3 balones de basquet

	Model casa2((char*)"Models/casa2/casa2.obj");// casita MIA
	
	Model car1((char*)"Models/car/car1.obj");// carro colores
	Model car2((char*)"Models/car/car2.obj");// carro colores
	Model carro3((char*)"Models/carro3/carro3.obj");// carro negro mio
	Model carro4madera((char*)"Models/carro4madera/carro4madera.obj");// carro negro mio

	Model piano1((char*)"Models/piano/piano1.obj");// piano
	Model piano2((char*)"Models/piano/piano2.obj");// 


	Model sonaja2((char*)"Models/sonaja2/sonaja2.obj");//


	////EXTRA---------------------------------------------------------------------------------
	Model caballo2((char*)"Models/caballo2/caballo2.obj");//
	Model alfombra((char*)"Models/alfombra/alfombra.obj");//
	Model camara((char*)"Models/camara/camara.obj");//
	Model camara2((char*)"Models/camara/camara2.obj");//

	Model jugueteamarillo((char*)"Models/jugueteamarillo/jugueteamarillo.obj");//
	Model mario((char*)"Models/mario/mario.obj");//
	Model osocar((char*)"Models/osocar/osocar.obj");//
	Model varios((char*)"Models/varios/varios.obj");//osos
	Model planta((char*)"Models/planta/planta.obj");//
	Model caja((char*)"Models/caja/caja.obj");//
	Model mesa4((char*)"Models/mesa4/mesa4.obj");//

	Model lego1y2((char*)"Models/legos/lego1y2.obj");//
	Model lego3y4((char*)"Models/legos/lego3y4.obj");//
	Model lego5y6y7((char*)"Models/legos/lego5y6y7.obj");//
	Model pecera((char*)"Models/pecera/pecera.obj");//
	Model peceravidrio((char*)"Models/pecera/peceravidrio.obj");//
	Model agua((char*)"Models/pecera/agua.obj");//

	Model lampara1((char*)"Models/lampara/lampara1.obj");//


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	/*glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);   */

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	
	
	//CaballoTorso = 0.0f;
	//CaballoTorso_offset = 0.005f;
	//CaballoPatasT = 0.0f;
	//CaballoPatasT_offset = -0.05f;
	//CaballoPatasD = 0.0f;
	//CaballoPatasD_offset = 0.05f;
	//CaballoCabeza = 0.0f;
	//CaballoCabeza_offset = 0.09f;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		//--------------------------------------PERRO
		/*CaballoCabeza += CaballoCabeza_offset;
		if (CaballoCabeza >= 25.0f || CaballoCabeza <= -25.0f)
			CaballoCabeza_offset = (CaballoCabeza_offset * (-1));

		CaballoPatasT += CaballoPatasT_offset;
		if (CaballoPatasT >= 45.0f || CaballoPatasT <= -45.0f)
			CaballoPatasT_offset = (CaballoPatasT_offset * (-1));

		CaballoPatasD += CaballoPatasD_offset;
		if (CaballoPatasD >= 45.0f || CaballoPatasD <= -45.0f)
			CaballoPatasD_offset = (CaballoPatasD_offset * (-1));

		CaballoTorso += 0.005f;*/

		//Load Model
		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light AMBIENTACION
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.9f, 0.9f, 0.9f);//ambiente
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);//ambiente
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.8f, 0.8f, 0.8f);

		// Point light 1
		glm::vec3 lightColor1;
		lightColor1.x = Light1.x;
		lightColor1.y = Light1.y;
		lightColor1.z = Light1.z;

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), 9.12f, 13.335f, 13.533f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor1.x, lightColor1.y, lightColor1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor1.x, lightColor1.y, lightColor1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.35f);//ilu sobre superficie
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.44f);//ILUMInacion sobre superficie

		// Point light 2
		glm::vec3 lightColor2;
		lightColor2.x = Light2.x;
		lightColor2.y = Light2.y;
		lightColor2.z = Light2.z;

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), -20.446f, 6.227f, 10.998f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.8f);

		// Point light 3
		glm::vec3 lightColor3;
		lightColor3.x = Light3.x;
		lightColor3.y = Light3.y;
		lightColor3.z = Light3.z;

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), -5.876f, 6.227f, 10.998f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 1.8f);

		// Point light 4
		glm::vec3 lightColor4;
		lightColor4.x = Light4.x;
		lightColor4.y = Light4.y;
		lightColor4.z = Light4.z;

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), -5.876f, 6.227f, 19.241f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 1.8f);
				
		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(0.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(0.0f)));
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);//brillo de objeto

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));




		glm::mat4 model(1);
		glm::mat4 modelaux(1.0);
		view = camera.GetViewMatrix();


		//FACHADACENTROCOMERCIAL--------------------------------------------------------------------------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		centrofachada.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hojacom.Draw(lightingShader);



		//CAFE INTERIOR Y EXTERIOR--------------------------------------------------------------------------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		exterior.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		interior.Draw(lightingShader);
		
		
		//-----------------------------------------------------------------------------------
	   

		////carro


		//CINE--------------------------------------------------------------------------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cine.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sala.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sala1.Draw(lightingShader);


		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		anicarro.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		anicarro2.Draw(lightingShader);

		////JUGUETERIA
		////Carga de modelos---------------------------------------------------------------------------------------------------
		
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		//fachada
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		fachada.Draw(lightingShader);


		//loquer DE ENTRADA
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		loquer.Draw(lightingShader);

		//escaleras
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		escaleras.Draw(lightingShader);

		//mueble 1 y 2 Y 3
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mueble.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mueble2.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mueble3.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sillon.Draw(lightingShader);

		//mesa de entrada
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa.Draw(lightingShader);

		//mesa de pecera
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa3.Draw(lightingShader);


		//7objetos----------------------------------------------------------------------------------------------- 

		//oso ROJO Y AZUL
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		oso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		osos.Draw(lightingShader);


		//balones TRES BASQUET
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		balon.Draw(lightingShader);

	    model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		balonitalia.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pelotafut.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Brazuka.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		carro3.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		carro4madera.Draw(lightingShader);
		
		
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		carro5lego.Draw(lightingShader);

		//ANIMACION DEL CARRO---------------------------------------------------------------
		//carro negro IA
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		car1.Draw(lightingShader);

		//carro negro IA
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.367f, 2.72f,14.44));//combinada
		model = glm::rotate(model, glm::radians(28.1f), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot2), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		car2.Draw(lightingShader);

		//------------------------------------------------------------------------------------


		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		casa2.Draw(lightingShader);

		//piano 1 y 2
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piano1.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piano2.Draw(lightingShader);

	

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sonaja2.Draw(lightingShader);

		////EXTRAAAA-----------------------------------------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		caballo2.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		alfombra.Draw(lightingShader);
		
		//ANIMACION DE CAMARA-------------------------------------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		camara.Draw(lightingShader);

		
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.882f, 5.972f, 6.286f));//combinada
		model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		camara2.Draw(lightingShader);



		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		jugueteamarillo.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mario.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		osocar.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		varios.Draw(lightingShader);
		
        model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		planta.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		caja.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa4.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lego1y2.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lego3y4.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lego5y6y7.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pecera.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lampara1.Draw(lightingShader);

		
		//TRANSPARENCIAS DE PUERTA---------------------------------------------------------------------
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
		puerta.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
		puerta2.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
		puertaarriba.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3( - 0.456f, 2.136f, 18.434f));//combinada
		model = glm::rotate(model, glm::radians(rot1), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
		puertac1.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.456f, 2.138f, 11.095));//combinada
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
		puertac2.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		//TRANSPARENCIAS DE VIDRIO DE MUEBLE---------------------------------------------------------------
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-des,0.0f,0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.35);
		vidriod.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-des1,0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.35);
		vidrioi.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		//vidrio de carro
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.35);
		carro3VIDRIO.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);


		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.35);
		peceravidrio.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);


		//vidrio de carro
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.25);
		interiorvidrio.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);


		//// Shader para simular el agua
		Anim.Use();
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//operaciones con canal alpha
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(Anim.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(Anim.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.15);
		tiempo = glfwGetTime() * 3.0f; // Definimos la intensidad del oleaje.
		//glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);

		agua.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		Anim.Use();
		
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tiempo = glfwGetTime() * 15.0f; // Definimos la intensidad del oleaje.
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		hoja.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 

		Anim.Use();

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tiempo = glfwGetTime() * 4.0f; // Definimos la intensidad del oleaje.
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		hojatallo.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 

		

		//--------------------------------------------------------------------------------------------------

			glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

	//ANIMACION PARA PUERTAS DE ENTRADA AMBAS---------------------------

	if (anim) {
		// Rotar puertac1 hacia afuera 
		if (rot < 90.0f) {
			rot += 0.4f;
		}
		// Rotar puertac2 hacia adentro 
		if (rot1 > -90.0f) {
			rot1 -= 0.4f;
		}
	}
	else {
		// Regresar puertac1 a 0
		if (rot > -0.0f) {
			rot -= 0.4f;
		}
		// Regresar puertac2 a 0
		if (rot1 < 0.0f) {
			rot1 += 0.4f;
		}
	}

	//ANIMACION PARA CAJUELA---------------------------------------
	if (anim1) {
		// Rotar puertac1 hacia afuera 
		if (rot2 < 60.0f) {
			rot2 += 0.4f;
		}

	}
	else {
		// Regresar puertac1 a 0
		if (rot2 > -0.0f) {
			rot2 -= 0.4f;
		}

	}

	//ANIMACION PARA CAMARA-----------------------------------------
	if (anim2)
	{
		if (direccion)
		{
			rot3 += 0.1f;
			if (rot3 >= 90.0f)
				direccion = false;
		}
		else
		{
			rot3 -= 0.1f;
			if (rot3 <= -5.0f)
				direccion = true;
		}
	}


	//ANIMACION PARA VIDRIO PUERTA DERECHA-------------------------------------
	if (anim3) {
		// Rotar puertac1 hacia afuera 
		if (des < 3.9f) {
			des += 0.01f;
		}

	}
	else {
		// Regresar puertac1 a 0
		if (des > -0.0f) {
			des -= 0.01f;
		}

	}
	
	//ANIMACION PARA VIDRIO PUERTA izquierda----------------------------------
	if (anim4) {
		// Rotar puertac1 hacia afuera 
		if (des1 < 4.6f) {
			des1 += 0.01f;
		}

	}
	else {
		// Regresar puertac1 a 0
		if (des1 > -0.0f) {
			des1 -= 0.01f;
		}

	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	//luz prende y apaga
	if (keys[GLFW_KEY_1])
	{
		// Alterna el valor de active cuando se presiona la tecla de espacio
		active = !active;

		// Solo procede si active es verdadero
		if (active)
		{
			// Desactiva todas las luces
			Light1 = glm::vec3(0);
			Light2 = glm::vec3(0);
			Light3 = glm::vec3(0);
			Light4 = glm::vec3(0);

			// Enciende la luz correspondiente según el índice actual
			switch (currentLightIndex)
			{
			case 0:
				Light1 = glm::vec3(1.0f, 1.0f, 1.0f); // 

				Light2 = glm::vec3(1.0f, 1.0f, 1.0f); // 

				Light3 = glm::vec3(1.0f, 1.0f, 1.0f); // 

				Light4 = glm::vec3(1.0f, 1.0f, 1.0f); // 

			}
		}
		else
		{
			// Si active es falso, apaga todas las luces
			Light1 = glm::vec3(0);
			Light2 = glm::vec3(0);
			Light3 = glm::vec3(0);
			Light4 = glm::vec3(0);
		}
	}

	//puertas
	if (keys[GLFW_KEY_2])
	{
		anim = !anim;

	}
	//cajuela
	if (keys[GLFW_KEY_3])
	{
		anim1 = !anim1;

	}
	//camara(es automaitco)
	if (keys[GLFW_KEY_4])
	{
		anim2 = true;

	}

	//vidrio dezlizante
	if (keys[GLFW_KEY_5])
	{
		anim3 = !anim3;

	}

	//vidrio dezlizante
	if (keys[GLFW_KEY_6])
	{
		anim4 = !anim4;

	}


	


}
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}