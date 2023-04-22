#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

#include "Model.h"
#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Intersection.h"

// to import blender obj models
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <sstream>
using namespace std;

using namespace glm;

Shader shader; // loads our vertex and fragment shaders

Model* pacMan;
Model* maze0;

Model* sphere1;
Model* sphere2;
Model* sphere3;
Model* sphere4;
Model* sphere5;
Model* sphere6;
Model* sphere7;
Model* sphere8;
Model* sphere9;

glm::mat4 projection; // projection matrix
glm::mat4 view; // where the camera is looking
glm::mat4 model; // where the model (i.e., the myModel) is located wrt the camera

float OBJ_DEPTH = -20.0f;
float PAC_MAN_SPEED = 0.1f;

vec3 mazePos = vec3(0.0f, 0.0f, OBJ_DEPTH);

// camera
vec4 center(0.0f, 0.0f, -15.0f, 1.0f);
vec4 eye; // camera position
vec4 cameraForward = vec4(0, 0, -1.0f, 0);
vec4 cameraUp = vec4(0, 1.0f, 0, 0);
vec4 cameraUpThreeD = vec4(0, 0, 1.0f, 0);
float cameraDistance = 38;
float cameraHeight = 5;

// pac man
float pacManXPos = 0.0f;
float pacManYPos = 0.0f;
float pacManXSpeed = -PAC_MAN_SPEED;
float pacManYSpeed = 0.0f;
float pacManRot = 0.0f;
std::string pacManCurDir = "left";
std::string pacManNextDir = "left";

// intersections
std::vector<Intersection> intersections;

// for rotating things
float dotProd;
float angleBetweenVectors;
vec4 newCameraCenter;
vec4 newCameraRight;
vec4 newCameraUp;
vec4 newCameraForward;
vec4 newPacManForward;
vec4 newPacManRight;
vec4 toOriginVector;
int score = 0;
int isFrozen = 0;
int ThreeD;


/* report GL errors, if any, to stderr */
void checkError(const char* functionName)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

void initShader(void)
{
	shader.InitializeFromFile("shaders/phong.vert", "shaders/blue.frag");
	shader.AddAttribute("vertexPosition");
	shader.AddAttribute("vertexNormal");

	checkError("initShader");
}

void initRendering(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	checkError("initRendering");
}

void init(void)
{
	// Perspective projection matrix.
	projection = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 1000.0f);

	// Load identity matrix into model matrix (no initial translation or rotation)


	initShader();
	initRendering();
}

/* This prints in the console when you start the program*/
void dumpInfo(void)
{
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	checkError("dumpInfo");
}

void turnCharacter(std::string character) {
	if (character == "pacman") {
		if (pacManCurDir == pacManNextDir) {
			pacManXPos += pacManXSpeed;
			pacManYPos += pacManYSpeed;
		}
		else if (pacManNextDir == "left") {
			pacManXSpeed = -PAC_MAN_SPEED;
			pacManYSpeed = 0.0f;
			pacManCurDir = pacManNextDir;
			pacManRot = 0;
		}
		else if (pacManNextDir == "right") {
			pacManXSpeed = PAC_MAN_SPEED;
			pacManYSpeed = 0.0f;
			pacManCurDir = pacManNextDir;
			pacManRot = 180;
		}
		else if (pacManNextDir == "up") {
			pacManXSpeed = 0.0f;
			pacManYSpeed = PAC_MAN_SPEED;
			pacManCurDir = pacManNextDir;
			pacManRot = -90;
		}
		else if (pacManNextDir == "down") {
			pacManXSpeed = 0.0f;
			pacManYSpeed = -PAC_MAN_SPEED;
			pacManCurDir = pacManNextDir;
			pacManRot = 90;
		}
	}
	/*else if () {
						// add ghost turning code here
	}*/
}

void adjustCharacter(std::string character, Intersection intersection) {
	if (character == "pacman") {
		pacManXPos = intersection.position.x;
		pacManYPos = intersection.position.y;
		if (pacManNextDir == "up") {
			pacManXSpeed = 0.0f;
			pacManYSpeed = PAC_MAN_SPEED;
			pacManYPos += pacManYSpeed;

		}
		else if (pacManNextDir == "down") {
			pacManXSpeed = 0.0f;
			pacManYSpeed = -PAC_MAN_SPEED;
			pacManYPos -= pacManYSpeed;

		}
		else if (pacManNextDir == "left") {
			pacManXSpeed = -PAC_MAN_SPEED;
			pacManYSpeed = 0.0f;
			pacManXPos -= pacManXSpeed;

		}
		else if (pacManNextDir == "right") {
			pacManXSpeed = PAC_MAN_SPEED;
			pacManYSpeed = 0.0f;
			pacManXPos += pacManXSpeed;

		}
	}
	//else if () {
						// add ghost adjusting code here
	//}
}

void freezeCharacter(std::string character) {
	if (character == "pacman") {
		pacManXSpeed = 0.0f;
		pacManYSpeed = 0.0f;
		isFrozen = 1;
	}
	//else if () {
						// add ghost freezing code here
	//}
}

bool contains(std::vector<std::string> dirs, std::string tgt) {
	for (int i = 0; i < dirs.size(); i++) {
		if (dirs[i] == tgt) {
			return true;
		}
	}
	return false;
}

float TURN_DETECTION_DISTANCE = 0.05f;

void detectTurn(std::string character, float objXPos, float objYPos, std::string objCurDir, std::string objNextDir) {
	vec3 objPos = vec3(objXPos, objYPos, OBJ_DEPTH);
	for (int i = 0; i < intersections.size(); i++) {
		Intersection curIntersection = intersections[i];
		float dist = glm::distance(objPos, curIntersection.position);
		if (dist < TURN_DETECTION_DISTANCE) { // check if we're close enough to the intersection to turn
			std::vector<std::string> curDirs = curIntersection.directions;
			if (contains(curDirs, objNextDir)) {
				adjustCharacter(character, curIntersection);
				turnCharacter(character);
				isFrozen = 0;
			}
			else {
				// if we try to turn into a wall, the character should not be translated
				freezeCharacter(character);
				//score--;
				break;
			}
		}
	}
}

void detectPellets(std::string character, float objXPos, float objYPos, std::string objCurDir, std::string objNextDir) {
	vec3 objPos = vec3(objXPos, objYPos, OBJ_DEPTH);
	for (int i = 0; i < intersections.size(); i++) {
		Intersection curIntersection = intersections[i];
		float dist = glm::distance(objPos, curIntersection.position);
		if (dist < TURN_DETECTION_DISTANCE && isFrozen != 1) { // check if we're close enough to the intersection to turn
			score++;
		}
	}
}

/*This gets called when the OpenGL is asked to display. This is where all the main rendering calls go*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: render differently based on game states

	// TODO: add different cameras
	

	// TODO: detect collisions with pellets
	// TODO: detect collisions with ghosts

	if (!ThreeD) {

		eye = center - cameraDistance * cameraForward;
		view = lookAt(vec3(eye), vec3(center), vec3(cameraUp));

		detectPellets("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);
		detectTurn("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);

		pacMan->render(view * translate(vec3(pacManXPos, pacManYPos, OBJ_DEPTH)) * rotate(pacManRot, 0.0f, 0.0f, 1.0f), projection, 4);

		maze0->render(view * translate(mazePos), projection, 1);

		sphere1->render(view * translate(-11.3403f, 10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere2->render(view * translate(0.0f, 10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere3->render(view * translate(11.3403f, 10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);

		sphere4->render(view * translate(-11.3403f, 0.0f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere5->render(view * translate(0.0f, 0.0f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere6->render(view * translate(11.3403f, 0.0f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);

		sphere7->render(view * translate(-11.3403f, -10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere8->render(view * translate(0.0f, -10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere9->render(view * translate(11.3403f, -10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);

		pacManXPos += pacManXSpeed;
		pacManYPos += pacManYSpeed;
	}
	else {
		view = lookAt(vec3(vec4(pacManXPos, pacManYPos, OBJ_DEPTH,0) - (rotate(pacManRot, 0.f, 0.f, 1.f) *
			vec4(-1, 0, 0, 0) * 10) + glm::vec4(0.0f, 0.f, 10, 0.f)), vec3(pacManXPos, pacManYPos, OBJ_DEPTH), vec3(cameraUpThreeD));

		detectPellets("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);
		detectTurn("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);

		pacMan->render(view * translate(vec3(pacManXPos, pacManYPos, OBJ_DEPTH)) * rotate(pacManRot, 0.0f, 0.0f, 1.0f), projection, 4);

		maze0->render(view * translate(mazePos), projection, 1);

		sphere1->render(view * translate(-11.3403f, 10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere2->render(view * translate(0.0f, 10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere3->render(view * translate(11.3403f, 10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);

		sphere4->render(view * translate(-11.3403f, 0.0f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere5->render(view * translate(0.0f, 0.0f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere6->render(view * translate(11.3403f, 0.0f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);

		sphere7->render(view * translate(-11.3403f, -10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere8->render(view * translate(0.0f, -10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);
		sphere9->render(view * translate(11.3403f, -10.1f, OBJ_DEPTH) * scale(0.5f, 0.5f, 0.5f), projection, 2);

		pacManXPos += pacManXSpeed;
		pacManYPos += pacManYSpeed;
	}

	glColor3f(0.0, 1.0, 0.0);
	glRasterPos2f(-0.97f, 0.9f);

	stringstream strs;
	strs << score;
	string temp = strs.str();
	char* scr_temp = (char*)temp.c_str();
	char* string = "SCORE: ";
	int i = 3;
	char* c;
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
	for (c = scr_temp; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}

	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2f(-0.1f, 0.85f);
	char* title = "PAC-MAN";
	for (c = title; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}

	glutSwapBuffers(); // Swap the buffers.
	checkError("display");
}

/*This gets called when nothing is happening (OFTEN)*/
void idle()
{
	glutPostRedisplay(); // create a display event. Display calls as fast as CPU will allow when put in the idle function
}

/*Called when the window is resized*/
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	checkError("reshape");
}

/*Called when a normal key is pressed*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: // this is an ascii value
		exit(0);
		break;
	case 'w':
		if (!ThreeD) {
			pacManNextDir = "up";
			if (pacManCurDir == "down") {
				turnCharacter("pacman");
			}
		}
		else {
			//nothing. In 3D we only turn.
		}
		break;
	case 's':
		if(!ThreeD){
			pacManNextDir = "down";
			if (pacManCurDir == "up") {
				turnCharacter("pacman");
			}
		}
		else {
			if (pacManCurDir == "up") {
				pacManNextDir = "down";
				turnCharacter("pacman");
			}
			else if (pacManCurDir == "down") {
				pacManNextDir = "up";
				turnCharacter("pacman");
			}
			else if (pacManCurDir == "right") {
				pacManNextDir = "left";
				turnCharacter("pacman");
			}
			else if (pacManCurDir == "left") {
				pacManNextDir = "right";
				turnCharacter("pacman");
			}
		}
		
		break;
	case 'a':
		if (!ThreeD) {
			pacManNextDir = "left";
			if (pacManCurDir == "right") {
				turnCharacter("pacman");
			}
		}
		else {
			if (pacManCurDir == "up") {
				pacManNextDir = "left";
			}
			else if (pacManCurDir == "down") {
				pacManNextDir = "right";
			}
			else if (pacManCurDir == "right") {
				pacManNextDir = "up";
			}
			else if (pacManCurDir == "left") {
				pacManNextDir = "down";
			}
		}
		break;
	case 'd':
		if (!ThreeD) {
			pacManNextDir = "right";
			if (pacManCurDir == "left") {
				turnCharacter("pacman");
			}
		}
		else {
			if (pacManCurDir == "up") {
				pacManNextDir = "right";
			}
			else if (pacManCurDir == "down") {
				pacManNextDir = "left";
			}
			else if (pacManCurDir == "right") {
				pacManNextDir = "down";
			}
			else if (pacManCurDir == "left") {
				pacManNextDir = "up";
			}
		}
		
		break;
	case 'f':
		if (ThreeD) {
			ThreeD = 0;
		}
		else {
			ThreeD = 1;
		}
		break;
	case 't':
		// use this key to print any output for debugging
		std::cout << std::endl;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glewInit();
	dumpInfo();
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);

	pacMan = new Model(&shader, "models/sphere.obj");
	maze0 = new Model(&shader, "models/maze0.obj", "models/");

	sphere1 = new Model(&shader, "models/sphere.obj");
	sphere2 = new Model(&shader, "models/sphere.obj");
	sphere3 = new Model(&shader, "models/sphere.obj");
	sphere4 = new Model(&shader, "models/sphere.obj");
	sphere5 = new Model(&shader, "models/sphere.obj");
	sphere6 = new Model(&shader, "models/sphere.obj");
	sphere7 = new Model(&shader, "models/sphere.obj");
	sphere8 = new Model(&shader, "models/sphere.obj");
	sphere9 = new Model(&shader, "models/sphere.obj");


	// top row
	std::vector<std::string> directions1 = { "down", "right" };
	std::vector<std::string> directions2 = { "left", "down", "right" };
	std::vector<std::string> directions3 = { "left", "down" };
	intersections.push_back(Intersection(vec3(-11.3403f, 10.1f, OBJ_DEPTH), directions1));
	intersections.push_back(Intersection(vec3(0.0f, 10.1f, OBJ_DEPTH), directions2));
	intersections.push_back(Intersection(vec3(11.3403f, 10.1f, OBJ_DEPTH), directions3));

	// middle row
	std::vector<std::string> directions4 = { "up", "down", "right" };
	std::vector<std::string> directions5 = { "up", "left", "down", "right" };
	std::vector<std::string> directions6 = { "up", "left", "down" };
	intersections.push_back(Intersection(vec3(-11.3403f, 0.0f, OBJ_DEPTH), directions4));
	intersections.push_back(Intersection(vec3(0.0f, 0.0f, OBJ_DEPTH), directions5));
	intersections.push_back(Intersection(vec3(11.3403f, 0.0f, OBJ_DEPTH), directions6));

	// bottom row
	std::vector<std::string> directions7 = { "up", "right" };
	std::vector<std::string> directions8 = { "left", "up", "right" };
	std::vector<std::string> directions9 = { "left", "up" };
	intersections.push_back(Intersection(vec3(-11.3403f, -10.1f, OBJ_DEPTH), directions7));
	intersections.push_back(Intersection(vec3(0.0f, -10.1f, OBJ_DEPTH), directions8));
	intersections.push_back(Intersection(vec3(11.3403f, -10.1f, OBJ_DEPTH), directions9));





	glutMainLoop();

	return 0;
}