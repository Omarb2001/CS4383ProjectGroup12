#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace glm;
glm::vec4 move(0.0f,0.0f, 0.0f,1.0f);
glm::vec4 freemove(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec3 center(0.0f,0.0f,0.0f);
Shader shader; // loads our vertex and fragment shaders
Model *cylinder; //a cylinder 
Model *plane; //a plane
Model* Head; //Head
Model* RA; //Right Hand
Model* LA; //Left Hand
Model* RL; //Right Leg
Model* LL; //Left Leg
Model* Garage;
Model *sphere; //a sphere
//Model *cube; //a cube
glm::mat4 projection; // projection matrix
glm::mat4 view; // where the camera is looking
glm::mat4 sphereTrans; // where the model is located wrt the camera
glm::mat4 cubeTrans; // where the model is located wrt the camera
glm::mat4 cylinderTrans; // where the model is located wrt the camera
glm::mat4 planeTrans; // where the model is located wrt the camera
glm::mat4 model; // where the model (i.e., the myModel) is located wrt the camera  //Torso
glm::mat4 LeftHand;
glm::mat4 RightHand;
glm::mat4 LeftLeg;
glm::mat4 RightLeg;
glm::mat4 garagemat;
vec4 lookatdirection = vec4(-1, 0, 0,0);
vec4 up = glm::vec4(0.0f, 0.0f, -1.0f,0.0f);
float cameradistance = 10;
float cameraheight = 10;
float angle1 = 0.0f;
float num = 0;
float num2 = 0;
int toggleFirstPerson = 1;
float freerotation = 0.f;
float freerotationvert = 0.f;
/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

void initShader(void)
{
	shader.InitializeFromFile("shaders/phong.vert", "shaders/phong.frag");
	shader.AddAttribute("vertexPosition");
	shader.AddAttribute("vertexNormal");

	checkError ("initShader");
}

void initRendering(void)
{
	glClearColor (0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	checkError ("initRendering");
}

void init(void) 
{	
	// Perspective projection matrix.
	projection = glm::perspective(45.0f, 800.0f/600.0f, 1.0f, 1000.0f);

	
	// Load identity matrix into model matrix (no initial translation or rotation)
	

	initShader ();
	initRendering ();
}

/* This prints in the console when you start the program*/
void dumpInfo(void)
{
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}
float rotation = 0.0f;
/*This gets called when the OpenGL is asked to display. This is where all the main rendering calls go*/
void display(void)
{

/*The transformation heirarchy is cylinder -> sphere -> cube*/
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the framebuffer data from the last frame

	//Initial camera: positioned at 15 on the z axis, looking into the screen down -z.
	
	
	if (toggleFirstPerson) {
		view = glm::lookAt(vec3(move - (rotate(rotation, 0.f, 0.f, 1.f) * lookatdirection * cameradistance) + glm::vec4(0.0f, 0.f, -cameraheight, 0.f)), vec3(move), vec3(up));
		model = translate(vec3(move)) * rotate(rotation, 0.f, 1.f, 0.f)*rotate(rotation,1.f,0.f,0.f);
		cylinder->renderPlane(view * model, projection);
	}
	else {
		view = glm::lookAt(vec3(0.f,0.f,-60.f), vec3(0.f,0.f,0.f), vec3(0.f,1.f,0.f));
		model = translate(vec3(move))*rotate(rotation, 0.f, 0.f, 1.f);
		cylinder->renderPlane(view * model, projection);
	}

	//sphereTrans = cylinderTrans*glm::rotate(0.0f, 0.0f, 0.0f, 1.0f)* glm::translate(0.0f, 2.f, -1.0f);
	//sphere->render(view * sphereTrans, projection); // Render the cube in another spot
	//draw ground
	
		planeTrans = view * glm::translate(0.0f, 0.0f, 0.0f);
		plane->renderPlane(planeTrans, projection);
	

	garagemat = view * translate(0.f, 0.f, 0.f);

	RightHand = glm::translate(1.6f, 1.95f, 0.75f) * glm::rotate(angle1, 1.0f, 0.0f, 0.0f) * glm::translate(-1.6f, -1.95f, -0.75f);
	LeftHand = glm::translate(-1.6f, 1.95f, 0.75f) * glm::rotate(-angle1, 1.0f, 0.0f, 0.0f) * glm::translate(1.6f, -1.95f, -0.75f);
	RightLeg = glm::translate(0.75f, -2.1f, 0.75f) * glm::rotate(-angle1, 1.0f, 0.0f, 0.0f) * glm::translate(-0.75f, 2.1f, -0.75f);
	LeftLeg = glm::translate(-0.75f, -2.1f, 0.75f) * glm::rotate(angle1, 1.0f, 0.0f, 0.0f) * glm::translate(0.75f, 2.1f, -0.75f);

	Garage->renderPlane(garagemat, projection);

	/*if (toggleFirstPerson) {
		//Head->renderHead(view * model, projection);
		RA->renderRA(model * RightHand, projection);
		LA->renderLA(model * LeftHand, projection);
		RL->renderRL(model * RightLeg, projection);
		LL->renderLL(model * LeftLeg, projection);
	}
	else {
		Head->renderHead(view * model, projection);
		RA->renderRA(view*model * RightHand, projection);
		LA->renderLA(view*model * LeftHand, projection);
		RL->renderRL(view*model * RightLeg, projection);
		LL->renderLL(view*model * LeftLeg, projection);
	}*/
	
	glutSwapBuffers(); // Swap the buffers.
	glutPostRedisplay();// create a display event.
	checkError ("display");
}

/*This gets called when nothing is happening (OFTEN)*/
void idle()
{
	  
}

/*Called when the window is resized*/
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	checkError ("reshape");
}

/*Called when a normal key is pressed*/
void keyboard(unsigned char key, int x, int y)
{
	vec4 lookatdir = rotate(rotation, 0.f, 0.f, 1.f)*lookatdirection;
	//glm::vec4 x1 = vec4(glm::normalize(glm::cross(vec3(up), vec3(lookatdir))),0.f);

	switch (key) {
	case 27: // this is an ascii value
		exit(0);
		break;
	case 'w':
		// move forward
		
		if (!toggleFirstPerson) {
			rotation = -90.f;
			move += lookatdir;
		}
		else {
			move += lookatdir;
		}
		break;
	case 'a':
		// turn left
		if (!toggleFirstPerson) {
			rotation = 180.f;
			move += lookatdir;
		}
		else {
			rotation -= 90;
		}
		break;
	case 's':
		if (!toggleFirstPerson) {
			rotation = 90.f;
			move += lookatdir;
		}
		else {
			move -= lookatdir;
		}
		break;
	case 'd':
		if (!toggleFirstPerson) {
			rotation = 0.f;
			move += lookatdir;
		}
		else {
			rotation += 90;
		}
		break;
	case 'c':
		if (toggleFirstPerson) {
			toggleFirstPerson = 0;
		}
		else {
			toggleFirstPerson = 1;
		}
		break;
	}
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	glewInit();
	dumpInfo ();
	init ();
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glEnable(GL_DEPTH_TEST);

	Garage = new Model(&shader, "models/garage.obj");
	cylinder = new Model(&shader, "models/cylinder.obj");
	plane = new Model(&shader, "models/maze1.obj", "models/");
	Head = new Model(&shader, "models/cylinder.obj"); //Head
	RA = new Model(&shader, "models/cylinder.obj");//Right Hand
	LA = new Model(&shader, "models/cylinder.obj");//Left Hand
	RL = new Model(&shader, "models/cylinder.obj");//Right Leg
	LL = new Model(&shader, "models/cylinder.obj");//Left Leg
	sphere = new Model(&shader, "models/sphere.obj");
	//cube = new Model(&shader, "models/unitcube.obj", "models/");

	rotation = -90.f;
	glutMainLoop();

	return 0;
}