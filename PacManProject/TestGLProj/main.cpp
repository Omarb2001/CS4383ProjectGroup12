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

Shader yellowShader; // loads our vertex and fragment shaders
Shader blueShader;
Shader redShader;
Shader cyanShader;
Shader orangeShader;
Shader pinkShader;
Shader shader;

Model* pacMan;
Model* maze0;
Model* redGhost;
Model* cyanGhost;
Model* orangeGhost;
Model* pinkGhost;
Model* cylinder;

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
float PAC_MAN_SPEED = 0.055f;
float GHOST_SPEED = 0.05f;
float SLOW_GHOSTS = 0.05f;;

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

// ghost1
float ghost1XPos = 11.3403f;
float ghost1YPos = 10.1f;
float ghost1XSpeed = -PAC_MAN_SPEED;
float ghost1YSpeed = 0.0f;
float ghost1Rot = 0.0f;
std::string ghost1CurDir = "left";
std::string ghost1NextDir = "left";

//ghost 2
float ghost2XPos = 11.3403f;
float ghost2YPos = -10.1f;
float ghost2XSpeed = -GHOST_SPEED;
float ghost2YSpeed = 0.0f;
float ghost2Rot = 0.0f;
std::string ghost2CurDir = "left";
std::string ghost2NextDir = "left";

// ghost3
float ghost3XPos = -11.3403f;
float ghost3YPos = -10.1f;
float ghost3XSpeed = -GHOST_SPEED;
float ghost3YSpeed = 0.0f;
float ghost3Rot = 0.0f;
std::string ghost3CurDir = "left";
std::string ghost3NextDir = "left";

// ghost4
float ghost4XPos = -11.3403f;
float ghost4YPos = 10.1f;
float ghost4XSpeed = -GHOST_SPEED;
float ghost4YSpeed = 0.0f;
float ghost4Rot = 0.0f;
std::string ghost4CurDir = "left";
std::string ghost4NextDir = "left";


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
int gameOverInt = 0;
int currentDist = 0;
int isFrozen = 0;
int ThreeD;

const int NUMPELLETS = 10;
vec3 pelletLoc3[NUMPELLETS];
int renderPellets[NUMPELLETS];

glm::vec4 lightPosition = glm::vec4(0.0f, 12.0f, OBJ_DEPTH + 3, 1.0f);
glm::vec4 spotlightPosition = glm::vec4(0.0f, 5.0f, OBJ_DEPTH + 6, 1.0f);
float rotation = 0.0f;


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
	yellowShader.InitializeFromFile("shaders/phong.vert", "shaders/yellow.frag");
	yellowShader.AddAttribute("vertexPosition");
	yellowShader.AddAttribute("vertexNormal");

	blueShader.InitializeFromFile("shaders/phong.vert", "shaders/blue.frag");
	blueShader.AddAttribute("vertexPosition");
	blueShader.AddAttribute("vertexNormal");

	redShader.InitializeFromFile("shaders/phong.vert", "shaders/red.frag");
	redShader.AddAttribute("vertexPosition");
	redShader.AddAttribute("vertexNormal");

	cyanShader.InitializeFromFile("shaders/phong.vert", "shaders/cyan.frag");
	cyanShader.AddAttribute("vertexPosition");
	cyanShader.AddAttribute("vertexNormal");

	orangeShader.InitializeFromFile("shaders/phong.vert", "shaders/orange.frag");
	orangeShader.AddAttribute("vertexPosition");
	orangeShader.AddAttribute("vertexNormal");

	pinkShader.InitializeFromFile("shaders/phong.vert", "shaders/pink.frag");
	pinkShader.AddAttribute("vertexPosition");
	pinkShader.AddAttribute("vertexNormal");

	shader.InitializeFromFile("shaders/phong.vert", "shaders/phong.frag");
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
	else if (character == "ghost1") {
		if (ghost1CurDir == ghost1NextDir) {
			ghost1XPos += ghost1XSpeed;
			ghost1YPos += ghost1YSpeed;
		}
		else if (ghost1NextDir == "left") {
			ghost1XSpeed = -GHOST_SPEED;
			ghost1YSpeed = 0.0f;
			ghost1CurDir = ghost1NextDir;
			ghost1Rot = 0;
		}
		else if (ghost1NextDir == "right") {
			ghost1XSpeed = GHOST_SPEED;
			ghost1YSpeed = 0.0f;
			ghost1CurDir = ghost1NextDir;
			ghost1Rot = 180;
		}
		else if (ghost1NextDir == "up") {
			ghost1XSpeed = 0.0f;
			ghost1YSpeed = GHOST_SPEED;
			ghost1CurDir = ghost1NextDir;
			ghost1Rot = -90;
		}
		else if (ghost1NextDir == "down") {
			ghost1XSpeed = 0.0f;
			ghost1YSpeed = -GHOST_SPEED;
			ghost1CurDir = ghost1NextDir;
			ghost1Rot = 90;
		}
	}
	else if (character == "ghost2") {
		if (ghost2CurDir == ghost2NextDir) {
			ghost2XPos += ghost2XSpeed;
			ghost2YPos += ghost2YSpeed;
		}
		else if (ghost2NextDir == "left") {
			ghost2XSpeed = -GHOST_SPEED;
			ghost2YSpeed = 0.0f;
			ghost2CurDir = ghost2NextDir;
			ghost2Rot = 0;
		}
		else if (ghost2NextDir == "right") {
			ghost2XSpeed = GHOST_SPEED;
			ghost2YSpeed = 0.0f;
			ghost2CurDir = ghost2NextDir;
			ghost2Rot = 180;
		}
		else if (ghost2NextDir == "up") {
			ghost2XSpeed = 0.0f;
			ghost2YSpeed = GHOST_SPEED;
			ghost2CurDir = ghost2NextDir;
			ghost2Rot = -90;
		}
		else if (ghost2NextDir == "down") {
			ghost2XSpeed = 0.0f;
			ghost2YSpeed = -GHOST_SPEED;
			ghost2CurDir = ghost2NextDir;
			ghost2Rot = 90;
		}

	}
	else if (character == "ghost3") {
		if (ghost3CurDir == ghost3NextDir) {
			ghost3XPos += ghost3XSpeed;
			ghost3YPos += ghost3YSpeed;
		}
		else if (ghost3NextDir == "left") {
			ghost3XSpeed = -GHOST_SPEED;
			ghost3YSpeed = 0.0f;
			ghost3CurDir = ghost3NextDir;
			ghost3Rot = 0;
		}
		else if (ghost3NextDir == "right") {
			ghost3XSpeed = GHOST_SPEED;
			ghost3YSpeed = 0.0f;
			ghost3CurDir = ghost3NextDir;
			ghost3Rot = 180;
		}
		else if (ghost3NextDir == "up") {
			ghost3XSpeed = 0.0f;
			ghost3YSpeed = GHOST_SPEED;
			ghost3CurDir = ghost3NextDir;
			ghost3Rot = -90;
		}
		else if (ghost3NextDir == "down") {
			ghost3XSpeed = 0.0f;
			ghost3YSpeed = -GHOST_SPEED;
			ghost3CurDir = ghost3NextDir;
			ghost3Rot = 90;
		}

	}
	else if (character == "ghost4") {
		if (ghost4CurDir == ghost4NextDir) {
			ghost4XPos += ghost4XSpeed;
			ghost4YPos += ghost4YSpeed;
		}
		else if (ghost4NextDir == "left") {
			ghost4XSpeed = -GHOST_SPEED;
			ghost4YSpeed = 0.0f;
			ghost4CurDir = ghost4NextDir;
			ghost4Rot = 0;
		}
		else if (ghost4NextDir == "right") {
			ghost4XSpeed = GHOST_SPEED;
			ghost4YSpeed = 0.0f;
			ghost4CurDir = ghost4NextDir;
			ghost4Rot = 180;
		}
		else if (ghost4NextDir == "up") {
			ghost4XSpeed = 0.0f;
			ghost4YSpeed = GHOST_SPEED;
			ghost4CurDir = ghost4NextDir;
			ghost4Rot = -90;
		}
		else if (ghost4NextDir == "down") {
			ghost4XSpeed = 0.0f;
			ghost4YSpeed = -GHOST_SPEED;
			ghost4CurDir = ghost4NextDir;
			ghost4Rot = 90;
		}

	}
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
	//ghost adjusting code here
	else if (character == "ghost1") {
		ghost1XPos = intersection.position.x;
		ghost1YPos = intersection.position.y;
		if (ghost1NextDir == "up") {
			ghost1XSpeed = 0.0f;
			ghost1YSpeed = GHOST_SPEED;
			ghost1YPos += ghost1YSpeed;

		}
		else if (ghost1NextDir == "down") {
			ghost1XSpeed = 0.0f;
			ghost1YSpeed = -GHOST_SPEED;
			ghost1YPos -= ghost1YSpeed;

		}
		else if (ghost1NextDir == "left") {
			ghost1XSpeed = -GHOST_SPEED;
			ghost1YSpeed = 0.0f;
			ghost1XPos -= ghost1XSpeed;

		}
		else if (ghost1NextDir == "right") {
			ghost1XSpeed = GHOST_SPEED;
			ghost1YSpeed = 0.0f;
			ghost1XPos += ghost1XSpeed;

		}
	}
	else if (character == "ghost2") {
		ghost2XPos = intersection.position.x;
		ghost2YPos = intersection.position.y;
		if (ghost2NextDir == "up") {
			ghost2XSpeed = 0.0f;
			ghost2YSpeed = GHOST_SPEED;
			ghost2YPos += ghost2YSpeed;

		}
		else if (ghost2NextDir == "down") {
			ghost2XSpeed = 0.0f;
			ghost2YSpeed = -GHOST_SPEED;
			ghost2YPos -= ghost2YSpeed;

		}
		else if (ghost2NextDir == "left") {
			ghost2XSpeed = -GHOST_SPEED;
			ghost2YSpeed = 0.0f;
			ghost2XPos -= ghost2XSpeed;

		}
		else if (ghost2NextDir == "right") {
			ghost2XSpeed = GHOST_SPEED;
			ghost2YSpeed = 0.0f;
			ghost2XPos += ghost2XSpeed;

		}
	}
	else if (character == "ghost3") {
		ghost3XPos = intersection.position.x;
		ghost3YPos = intersection.position.y;
		if (ghost3NextDir == "up") {
			ghost3XSpeed = 0.0f;
			ghost3YSpeed = GHOST_SPEED;
			ghost3YPos += ghost3YSpeed;

		}
		else if (ghost3NextDir == "down") {
			ghost3XSpeed = 0.0f;
			ghost3YSpeed = -GHOST_SPEED;
			ghost3YPos -= ghost3YSpeed;

		}
		else if (ghost3NextDir == "left") {
			ghost3XSpeed = -GHOST_SPEED;
			ghost3YSpeed = 0.0f;
			ghost3XPos -= ghost3XSpeed;

		}
		else if (ghost3NextDir == "right") {
			ghost3XSpeed = GHOST_SPEED;
			ghost3YSpeed = 0.0f;
			ghost3XPos += ghost3XSpeed;

		}
	}
	else if (character == "ghost4") {
		ghost4XPos = intersection.position.x;
		ghost4YPos = intersection.position.y;
		if (ghost4NextDir == "up") {
			ghost4XSpeed = 0.0f;
			ghost4YSpeed = GHOST_SPEED;
			ghost4YPos += ghost4YSpeed;

		}
		else if (ghost4NextDir == "down") {
			ghost4XSpeed = 0.0f;
			ghost4YSpeed = -GHOST_SPEED;
			ghost4YPos -= ghost4YSpeed;

		}
		else if (ghost4NextDir == "left") {
			ghost4XSpeed = -GHOST_SPEED;
			ghost4YSpeed = 0.0f;
			ghost4XPos -= ghost4XSpeed;

		}
		else if (ghost4NextDir == "right") {
			ghost4XSpeed = GHOST_SPEED;
			ghost4YSpeed = 0.0f;
			ghost4XPos += ghost4XSpeed;

		}
	}
}

void freezeCharacter(std::string character) {
	if (character == "pacman") {
		pacManXSpeed = 0.0f;
		pacManYSpeed = 0.0f;
		isFrozen = 1;
	}
	//ghost freezing code here
	else if (character == "ghost1") {
		ghost1XSpeed = 0.0f;
		ghost1YSpeed = 0.0f;
		isFrozen = 1;
	}
	else if (character == "ghost2") {
		ghost2XSpeed = 0.0f;
		ghost2YSpeed = 0.0f;
		isFrozen = 1;
	}
	else if (character == "ghost3") {
		ghost3XSpeed = 0.0f;
		ghost3YSpeed = 0.0f;
		isFrozen = 1;
	}
	else if (character == "ghost4") {
		ghost4XSpeed = 0.0f;
		ghost4YSpeed = 0.0f;
		isFrozen = 1;
	}
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

void detectPellets(std::string character, float objXPos, float objYPos) {
	vec3 objPos = vec3(objXPos, objYPos, OBJ_DEPTH);
	
	float pelletsDist[NUMPELLETS];

	for (int i = 1; i < NUMPELLETS; i++) {
		pelletsDist[i] = glm::distance(objPos, pelletLoc3[i]);
	}

	for (int i = 1; i < NUMPELLETS; i++) {
		if (renderPellets[i] == 0 && pelletsDist[i] < 1.5f) {
			renderPellets[i] = 1;
			score += 10;
		}
	}

	if (score == 90) {
		gameOverInt = 1000;
	}
}

//Check if game over
void gameOver(std::string character, std::string character1, float objXPos, float objYPos, float obj1XPos, float obj1YPos) {
	vec3 objPos = vec3(objXPos, objYPos, OBJ_DEPTH);
	vec3 obj1Pos = vec3(obj1XPos, obj1YPos, OBJ_DEPTH);
	
		if (character1 == "ghost1") {
			float enemy1_dist = glm::distance(objPos, obj1Pos);
			//check if close enough to ghost1
			if (enemy1_dist < 2.0f) {
				gameOverInt++;
			}
		}
		else if (character1 == "ghost2") {
			float enemy2_dist = glm::distance(objPos, obj1Pos);
			//check if close enough to ghost1
			if (enemy2_dist < 2.0f) {
				gameOverInt++;
			}
		}
		else if (character1 == "ghost3") {
			float enemy3_dist = glm::distance(objPos, obj1Pos);
			//check if close enough to ghost1
			if (enemy3_dist < 2.0f) {
				gameOverInt++;
			}
		}
		else if (character1 == "ghost4") {
			float enemy4_dist = glm::distance(objPos, obj1Pos);
			//check if close enough to ghost1
			if (enemy4_dist < 2.0f) {
				gameOverInt++;
			}
		}
	
}

void ghostAI(std::string character, std::string character1, float objXPos, float objYPos, float obj1XPos, float obj1YPos, std::string objCurDir, std::string objNextDir) {
	vec3 objPos = vec3(objXPos, objYPos, OBJ_DEPTH);
	vec3 obj1Pos = vec3(obj1XPos, obj1YPos, OBJ_DEPTH);
	
		for (int i = 0; i < intersections.size(); i++) {
			Intersection curIntersection = intersections[i];
			float turn1_dist = glm::distance(obj1Pos, curIntersection.position);
			if (character1 == "ghost1") {
				if (turn1_dist < TURN_DETECTION_DISTANCE) { // check if we're close enough to the intersection to turn
					std::vector<std::string> curDirs1 = curIntersection.directions;
					if (contains(curDirs1, objNextDir)) {
						adjustCharacter(character1, curIntersection);
						turnCharacter(character1);
						isFrozen = 0;
						int possibleTurns1 = curDirs1.size();
						srand((unsigned)time(0));
						int index1;
						index1 = (rand() % possibleTurns1);
						ghost1NextDir = curDirs1[index1];
					}
					else {
						// if we try to turn into a wall, the character should not be translated
						freezeCharacter(character1);
						int possibleTurns1 = curDirs1.size();
						srand((unsigned)time(0));
						int index1;
						index1 = (rand() % possibleTurns1);
						ghost1NextDir = curDirs1[index1];
						break;
					}
				}
			}
			else if (character1 == "ghost2") {
				float turn2_dist = glm::distance(obj1Pos, curIntersection.position);
				if (turn2_dist < TURN_DETECTION_DISTANCE) { // check if we're close enough to the intersection to turn
					std::vector<std::string> curDirs2 = curIntersection.directions;
					if (contains(curDirs2, objNextDir)) {
						adjustCharacter(character1, curIntersection);
						turnCharacter(character1);
						isFrozen = 0;
						int possibleTurns2 = curDirs2.size();
						srand((unsigned)time(0));
						int index2;
						index2 = (rand() % possibleTurns2);
						ghost2NextDir = curDirs2[index2];
					}
					else {
						// if we try to turn into a wall, the character should not be translated
						freezeCharacter(character1);
						int possibleTurns2 = curDirs2.size();
						srand((unsigned)time(0));
						int index2;
						index2 = (rand() % possibleTurns2);
						ghost2NextDir = curDirs2[index2];
						break;
					}
				}
			}
			else if (character1 == "ghost3") {
				float turn3_dist = glm::distance(obj1Pos, curIntersection.position);
				if (turn3_dist < TURN_DETECTION_DISTANCE) { // check if we're close enough to the intersection to turn
					std::vector<std::string> curDirs3 = curIntersection.directions;
					if (contains(curDirs3, objNextDir)) {
						adjustCharacter(character1, curIntersection);
						turnCharacter(character1);
						isFrozen = 0;
						int possibleTurns3 = curDirs3.size();
						srand((unsigned)time(0));
						int index3;
						index3 = (rand() * 3 % possibleTurns3);
						ghost3NextDir = curDirs3[index3];
					}
					else {
						// if we try to turn into a wall, the character should not be translated
						freezeCharacter(character1);
						int possibleTurns3 = curDirs3.size();
						srand((unsigned)time(0));
						int index3;
						index3 = (rand() * 3 % possibleTurns3);
						ghost3NextDir = curDirs3[index3];
						break;
					}
				}
			}
			else if (character1 == "ghost4") {
				float turn4_dist = glm::distance(obj1Pos, curIntersection.position);
				if (turn4_dist < TURN_DETECTION_DISTANCE) { // check if we're close enough to the intersection to turn
					std::vector<std::string> curDirs4 = curIntersection.directions;
					if (contains(curDirs4, objNextDir)) {
						adjustCharacter(character1, curIntersection);
						turnCharacter(character1);
						isFrozen = 0;
						int possibleTurns4 = curDirs4.size();
						srand((unsigned)time(0));
						int index4;
						index4 = (rand() * 2 % possibleTurns4);
						ghost4NextDir = curDirs4[index4];
					}
					else {
						// if we try to turn into a wall, the character should not be translated
						freezeCharacter(character1);
						int possibleTurns4 = curDirs4.size();
						srand((unsigned)time(0));
						int index4;
						index4 = (rand() * 2 % possibleTurns4);
						ghost4NextDir = curDirs4[index4];
						break;
					}
				}

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
	if (gameOverInt <= 200) {
		if (!ThreeD) {

			eye = center - cameraDistance * cameraForward;
			view = lookAt(vec3(eye), vec3(center), vec3(cameraUp));

			detectPellets("pacman", pacManXPos, pacManYPos);
			detectTurn("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);
			gameOver("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos);
			gameOver("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos);
			gameOver("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos);
			gameOver("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos);
			ghostAI("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos, ghost1CurDir, ghost1NextDir);
			ghostAI("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos, ghost2CurDir, ghost2NextDir);
			ghostAI("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos, ghost3CurDir, ghost3NextDir);
			ghostAI("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos, ghost4CurDir, ghost4NextDir);


			glm::vec4 lightPos = glm::rotate(rotation, 0.0f, 0.0f, 1.0f) * lightPosition;
			shader.Activate(); // Bind shader.
			shader.SetUniform("lightPosition", view * lightPos);
			shader.SetUniform("lightDiffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			shader.SetUniform("lightSpecular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			shader.SetUniform("lightAmbient", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			shader.SetUniform("cuttoffAngle", 30.0f);

			shader.SetUniform("spotlightPosition", spotlightPosition);
			shader.SetUniform("spotlightDirection", glm::vec4(0.0, 1.0, 0.0, 0.0));
			shader.SetUniform("spotlightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
			shader.SetUniform("spotlightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
			shader.SetUniform("spotlightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
			shader.SetUniform("cuttoffAngle", 40.0f);



			pacMan->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 0.0, 1.0));
			pacMan->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			pacMan->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
			pacMan->setOverrideSpecularShininessMaterial(90.0f);
			pacMan->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			pacMan->render(view * translate(vec3(pacManXPos, pacManYPos, OBJ_DEPTH)) * rotate(pacManRot, 0.0f, 0.0f, 1.0f), projection);

			redGhost->render(view * translate(vec3(ghost1XPos, ghost1YPos, OBJ_DEPTH)) * rotate(ghost1Rot, 0.0f, 0.0f, 1.0f), projection);
			cyanGhost->render(view * translate(vec3(ghost2XPos, ghost2YPos, OBJ_DEPTH)) * rotate(ghost2Rot, 0.0f, 0.0f, 1.0f), projection);
			orangeGhost->render(view * translate(vec3(ghost3XPos, ghost3YPos, OBJ_DEPTH)) * rotate(ghost3Rot, 0.0f, 0.0f, 1.0f), projection);
			pinkGhost->render(view * translate(vec3(ghost4XPos, ghost4YPos, OBJ_DEPTH)) * rotate(ghost4Rot, 0.0f, 0.0f, 1.0f), projection);


			maze0->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));
			maze0->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			maze0->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
			maze0->setOverrideSpecularShininessMaterial(90.0f);
			maze0->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			maze0->render(view * translate(mazePos), projection);

			if (renderPellets[1] == 0)
				sphere1->render(view * translate(pelletLoc3[1]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[2] == 0)
				sphere2->render(view * translate(pelletLoc3[2]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[3] == 0)
				sphere3->render(view * translate(pelletLoc3[3]) * scale(0.5f, 0.5f, 0.5f), projection);

			if (renderPellets[4] == 0)
				sphere4->render(view * translate(pelletLoc3[4]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[5] == 0)
				sphere5->render(view * translate(pelletLoc3[5]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[6] == 0)
				sphere6->render(view * translate(pelletLoc3[6]) * scale(0.5f, 0.5f, 0.5f), projection);

			if (renderPellets[7] == 0)
				sphere7->render(view * translate(pelletLoc3[7]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[8] == 0)
				sphere8->render(view * translate(pelletLoc3[8]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[9] == 0)
				sphere9->render(view * translate(pelletLoc3[9]) * scale(0.5f, 0.5f, 0.5f), projection);

			pacManXPos += pacManXSpeed;
			pacManYPos += pacManYSpeed;
			ghost1XPos += ghost1XSpeed;
			ghost1YPos += ghost1YSpeed;
			ghost2XPos += ghost2XSpeed;
			ghost2YPos += ghost2YSpeed;
			ghost3XPos += ghost3XSpeed;
			ghost3YPos += ghost3YSpeed;
			ghost4XPos += ghost4XSpeed;
			ghost4YPos += ghost4YSpeed;
		}
		else {
			view = lookAt(vec3(vec4(pacManXPos, pacManYPos, OBJ_DEPTH, 0) - (rotate(pacManRot, 0.f, 0.f, 1.f) *
				vec4(-1, 0, 0, 0) * 10) + glm::vec4(0.0f, 0.f, 10, 0.f)), vec3(pacManXPos, pacManYPos, OBJ_DEPTH), vec3(cameraUpThreeD));


			glm::vec4 spotModel = glm::vec4(0.0f, 5.0f, OBJ_DEPTH, 0.0f) * glm::rotate(0.0f, 90.0f, 1.0f, 0.0f);
			rotation += 0.05f; // Update rotation angle if rotation is enabled.

			glm::vec4 lightPos = glm::rotate(rotation, 0.0f, 0.0f, 1.0f) * lightPosition;

			shader.Activate(); // Bind shader.
			shader.SetUniform("lightPosition", view * lightPos);
			shader.SetUniform("lightDiffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			shader.SetUniform("lightSpecular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			shader.SetUniform("lightAmbient", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			shader.SetUniform("cuttoffAngle", 30.0f);

			shader.SetUniform("spotlightPosition", spotlightPosition);
			shader.SetUniform("spotlightDirection", glm::vec4(0.0, 1.0, 0.0, 0.0));
			shader.SetUniform("spotlightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
			shader.SetUniform("spotlightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
			shader.SetUniform("spotlightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
			shader.SetUniform("cuttoffAngle", 40.0f);


			cylinder->setOverrideDiffuseMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0));
			cylinder->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			cylinder->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
			cylinder->setOverrideSpecularShininessMaterial(90.0f);
			cylinder->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			//cylinder->render(glm::translate(0.0f, 5.0f, OBJ_DEPTH), projection);

			detectPellets("pacman", pacManXPos, pacManYPos);
			detectTurn("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);

			gameOver("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos);
			gameOver("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos);
			gameOver("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos);
			gameOver("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos);
			ghostAI("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos, ghost1CurDir, ghost1NextDir);
			ghostAI("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos, ghost2CurDir, ghost2NextDir);
			ghostAI("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos, ghost3CurDir, ghost3NextDir);
			ghostAI("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos, ghost4CurDir, ghost4NextDir);


			pacMan->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 0.0, 1.0));
			pacMan->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			pacMan->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
			pacMan->setOverrideSpecularShininessMaterial(90.0f);
			pacMan->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			pacMan->render(view * translate(vec3(pacManXPos, pacManYPos, OBJ_DEPTH)) * rotate(pacManRot, 0.0f, 0.0f, 1.0f), projection);

			redGhost->render(view * translate(vec3(ghost1XPos, ghost1YPos, OBJ_DEPTH)) * rotate(ghost1Rot, 0.0f, 0.0f, 1.0f), projection);
			cyanGhost->render(view * translate(vec3(ghost2XPos, ghost2YPos, OBJ_DEPTH)) * rotate(ghost2Rot, 0.0f, 0.0f, 1.0f), projection);
			orangeGhost->render(view * translate(vec3(ghost3XPos, ghost3YPos, OBJ_DEPTH)) * rotate(ghost3Rot, 0.0f, 0.0f, 1.0f), projection);
			pinkGhost->render(view * translate(vec3(ghost4XPos, ghost4YPos, OBJ_DEPTH)) * rotate(ghost4Rot, 0.0f, 0.0f, 1.0f), projection);


			maze0->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));
			maze0->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			maze0->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
			maze0->setOverrideSpecularShininessMaterial(90.0f);
			maze0->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
			maze0->render(view * translate(mazePos), projection);

			if (renderPellets[1] == 0)
				sphere1->render(view * translate(pelletLoc3[1]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[2] == 0)
				sphere2->render(view * translate(pelletLoc3[2]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[3] == 0)
				sphere3->render(view * translate(pelletLoc3[3]) * scale(0.5f, 0.5f, 0.5f), projection);

			if (renderPellets[4] == 0)
				sphere4->render(view * translate(pelletLoc3[4]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[5] == 0)
				sphere5->render(view * translate(pelletLoc3[5]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[6] == 0)
				sphere6->render(view * translate(pelletLoc3[6]) * scale(0.5f, 0.5f, 0.5f), projection);

			if (renderPellets[7] == 0)
				sphere7->render(view * translate(pelletLoc3[7]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[8] == 0)
				sphere8->render(view * translate(pelletLoc3[8]) * scale(0.5f, 0.5f, 0.5f), projection);
			if (renderPellets[9] == 0)
				sphere9->render(view * translate(pelletLoc3[9]) * scale(0.5f, 0.5f, 0.5f), projection);

			pacManXPos += pacManXSpeed;
			pacManYPos += pacManYSpeed;
			ghost1XPos += ghost1XSpeed;
			ghost1YPos += ghost1YSpeed;
			ghost2XPos += ghost2XSpeed;
			ghost2YPos += ghost2YSpeed;
			ghost3XPos += ghost3XSpeed;
			ghost3YPos += ghost3YSpeed;
			ghost4XPos += ghost4XSpeed;
			ghost4YPos += ghost4YSpeed;




		}


		glColor3f(0.0, 1.0, 0.0);
		glRasterPos2f(-0.97f, 0.9f);

		stringstream strs;
		strs << score;
		std::string temp = strs.str();
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

		//check if ghost touches pacman
		glColor3f(0.0, 1.0, 0.0);
		glRasterPos2f(0.55f, 0.9f);
		stringstream strs1;
		strs1 << gameOverInt;
		temp = strs1.str();
		scr_temp = (char*)temp.c_str();
		string = "Time in Contact w/ Ghost: ";
		i = 5;
		for (c = string; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		}
		for (c = scr_temp; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		}
		//current dist ghost is from pacman
		glColor3f(0.0, 1.0, 0.0);
		glRasterPos2f(0.7f, 0.85f);
		stringstream strs2;
		strs2 << ghost3NextDir;
		temp = strs2.str();
		scr_temp = (char*)temp.c_str();
		string = "ghost1NextDir: ";
		i = 5;
		//for (c = string; *c != '\0'; c++) {
		//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		//}
		//for (c = scr_temp; *c != '\0'; c++) {
		//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		//}
		glutSwapBuffers(); // Swap the buffers.
		checkError("display");
	}
	else {
		if (!ThreeD) {

				eye = center - cameraDistance * cameraForward;
				view = lookAt(vec3(eye), vec3(center), vec3(cameraUp));

				detectPellets("pacman", pacManXPos, pacManYPos);
				detectTurn("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);
				gameOver("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos);
				gameOver("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos);
				gameOver("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos);
				gameOver("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos);
				ghostAI("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos, ghost1CurDir, ghost1NextDir);
				ghostAI("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos, ghost2CurDir, ghost2NextDir);
				ghostAI("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos, ghost3CurDir, ghost3NextDir);
				ghostAI("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos, ghost4CurDir, ghost4NextDir);


				glm::vec4 lightPos = glm::rotate(rotation, 0.0f, 0.0f, 1.0f) * lightPosition;
				shader.Activate(); // Bind shader.
				shader.SetUniform("lightPosition", view * lightPos);
				shader.SetUniform("lightDiffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				shader.SetUniform("lightSpecular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				shader.SetUniform("lightAmbient", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				shader.SetUniform("cuttoffAngle", 30.0f);

				shader.SetUniform("spotlightPosition", spotlightPosition);
				shader.SetUniform("spotlightDirection", glm::vec4(0.0, 1.0, 0.0, 0.0));
				shader.SetUniform("spotlightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
				shader.SetUniform("spotlightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
				shader.SetUniform("spotlightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
				shader.SetUniform("cuttoffAngle", 40.0f);



				pacMan->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 0.0, 1.0));
				pacMan->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
				pacMan->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
				pacMan->setOverrideSpecularShininessMaterial(90.0f);
				pacMan->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
				pacMan->render(view * translate(pelletLoc3[5]) * rotate(pacManRot, 0.0f, 0.0f, 1.0f), projection);

				redGhost->render(view * translate(pelletLoc3[1]) * rotate(ghost1Rot, 0.0f, 0.0f, 1.0f), projection);
				cyanGhost->render(view * translate(pelletLoc3[3]) * rotate(ghost2Rot, 0.0f, 0.0f, 1.0f), projection);
				orangeGhost->render(view * translate(pelletLoc3[7]) * rotate(ghost3Rot, 0.0f, 0.0f, 1.0f), projection);
				pinkGhost->render(view * translate(pelletLoc3[9]) * rotate(ghost4Rot, 0.0f, 0.0f, 1.0f), projection);


				maze0->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));
				maze0->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
				maze0->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
				maze0->setOverrideSpecularShininessMaterial(90.0f);
				maze0->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
				maze0->render(view * translate(mazePos), projection);

				if (renderPellets[1] == 0)
					sphere1->render(view * translate(pelletLoc3[1]) * scale(0.5f, 0.5f, 0.5f), projection);
				if (renderPellets[2] == 0)
					sphere2->render(view * translate(pelletLoc3[2]) * scale(0.5f, 0.5f, 0.5f), projection);
				if (renderPellets[3] == 0)
					sphere3->render(view * translate(pelletLoc3[3]) * scale(0.5f, 0.5f, 0.5f), projection);

				if (renderPellets[4] == 0)
					sphere4->render(view * translate(pelletLoc3[4]) * scale(0.5f, 0.5f, 0.5f), projection);
				if (renderPellets[5] == 0)
					sphere5->render(view * translate(pelletLoc3[5]) * scale(0.5f, 0.5f, 0.5f), projection);
				if (renderPellets[6] == 0)
					sphere6->render(view * translate(pelletLoc3[6]) * scale(0.5f, 0.5f, 0.5f), projection);

				if (renderPellets[7] == 0)
					sphere7->render(view * translate(pelletLoc3[7]) * scale(0.5f, 0.5f, 0.5f), projection);
				if (renderPellets[8] == 0)
					sphere8->render(view * translate(pelletLoc3[8]) * scale(0.5f, 0.5f, 0.5f), projection);
				if (renderPellets[9] == 0)
					sphere9->render(view * translate(pelletLoc3[9]) * scale(0.5f, 0.5f, 0.5f), projection);


				freezeCharacter("pacman");
				freezeCharacter("ghost1");
				freezeCharacter("ghost2");
				freezeCharacter("ghost3");
				freezeCharacter("ghost4");

				pacManXPos = 0.0f;
				pacManYPos = 0.0f;
				ghost1XPos = -11.3403f;
				ghost1YPos = 10.1f;
				ghost2XPos = 11.3403f;
				ghost2YPos = 10.1f;
				ghost3XPos = -11.3403f;
				ghost3YPos = -10.1f;
				ghost4XPos = 11.3403f;
				ghost4YPos = -10.1f;

				for (int i = 1; i < NUMPELLETS; i++) {
					renderPellets[i] = 1;
				}
				score = 0;

			}
		else {
		view = lookAt(vec3(vec4(pacManXPos, pacManYPos, OBJ_DEPTH, 0) - (rotate(pacManRot, 0.f, 0.f, 1.f) *
			vec4(-1, 0, 0, 0) * 10) + glm::vec4(0.0f, 0.f, 10, 0.f)), vec3(pacManXPos, pacManYPos, OBJ_DEPTH), vec3(cameraUpThreeD));


		glm::vec4 spotModel = glm::vec4(0.0f, 5.0f, OBJ_DEPTH, 0.0f) * glm::rotate(0.0f, 90.0f, 1.0f, 0.0f);
		rotation += 0.05f; // Update rotation angle if rotation is enabled.

		glm::vec4 lightPos = glm::rotate(rotation, 0.0f, 0.0f, 1.0f) * lightPosition;

		shader.Activate(); // Bind shader.
		shader.SetUniform("lightPosition", view * lightPos);
		shader.SetUniform("lightDiffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader.SetUniform("lightSpecular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader.SetUniform("lightAmbient", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		shader.SetUniform("cuttoffAngle", 30.0f);

		shader.SetUniform("spotlightPosition", spotlightPosition);
		shader.SetUniform("spotlightDirection", glm::vec4(0.0, 1.0, 0.0, 0.0));
		shader.SetUniform("spotlightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
		shader.SetUniform("spotlightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
		shader.SetUniform("spotlightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
		shader.SetUniform("cuttoffAngle", 40.0f);


		cylinder->setOverrideDiffuseMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0));
		cylinder->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		cylinder->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		cylinder->setOverrideSpecularShininessMaterial(90.0f);
		cylinder->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		//cylinder->render(glm::translate(0.0f, 5.0f, OBJ_DEPTH), projection);

		detectPellets("pacman", pacManXPos, pacManYPos);
		detectTurn("pacman", pacManXPos, pacManYPos, pacManCurDir, pacManNextDir);

		gameOver("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos);
		gameOver("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos);
		gameOver("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos);
		gameOver("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos);
		ghostAI("pacman", "ghost1", pacManXPos, pacManYPos, ghost1XPos, ghost1YPos, ghost1CurDir, ghost1NextDir);
		ghostAI("pacman", "ghost2", pacManXPos, pacManYPos, ghost2XPos, ghost2YPos, ghost2CurDir, ghost2NextDir);
		ghostAI("pacman", "ghost3", pacManXPos, pacManYPos, ghost3XPos, ghost3YPos, ghost3CurDir, ghost3NextDir);
		ghostAI("pacman", "ghost4", pacManXPos, pacManYPos, ghost4XPos, ghost4YPos, ghost4CurDir, ghost4NextDir);


		pacMan->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 0.0, 1.0));
		pacMan->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		pacMan->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		pacMan->setOverrideSpecularShininessMaterial(90.0f);
		pacMan->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		pacMan->render(view * translate(vec3(pacManXPos, pacManYPos, OBJ_DEPTH)) * rotate(pacManRot, 0.0f, 0.0f, 1.0f), projection);

		redGhost->render(view * translate(vec3(ghost1XPos, ghost1YPos, OBJ_DEPTH)) * rotate(ghost1Rot, 0.0f, 0.0f, 1.0f), projection);
		cyanGhost->render(view * translate(vec3(ghost2XPos, ghost2YPos, OBJ_DEPTH)) * rotate(ghost2Rot, 0.0f, 0.0f, 1.0f), projection);
		orangeGhost->render(view * translate(vec3(ghost3XPos, ghost3YPos, OBJ_DEPTH)) * rotate(ghost3Rot, 0.0f, 0.0f, 1.0f), projection);
		pinkGhost->render(view * translate(vec3(ghost4XPos, ghost4YPos, OBJ_DEPTH)) * rotate(ghost4Rot, 0.0f, 0.0f, 1.0f), projection);


		maze0->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));
		maze0->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		maze0->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		maze0->setOverrideSpecularShininessMaterial(90.0f);
		maze0->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		maze0->render(view * translate(mazePos), projection);

		if (renderPellets[1] == 0)
			sphere1->render(view * translate(pelletLoc3[1]) * scale(0.5f, 0.5f, 0.5f), projection);
		if (renderPellets[2] == 0)
			sphere2->render(view * translate(pelletLoc3[2]) * scale(0.5f, 0.5f, 0.5f), projection);
		if (renderPellets[3] == 0)
			sphere3->render(view * translate(pelletLoc3[3]) * scale(0.5f, 0.5f, 0.5f), projection);

		if (renderPellets[4] == 0)
			sphere4->render(view * translate(pelletLoc3[4]) * scale(0.5f, 0.5f, 0.5f), projection);
		if (renderPellets[5] == 0)
			sphere5->render(view * translate(pelletLoc3[5]) * scale(0.5f, 0.5f, 0.5f), projection);
		if (renderPellets[6] == 0)
			sphere6->render(view * translate(pelletLoc3[6]) * scale(0.5f, 0.5f, 0.5f), projection);

		if (renderPellets[7] == 0)
			sphere7->render(view * translate(pelletLoc3[7]) * scale(0.5f, 0.5f, 0.5f), projection);
		if (renderPellets[8] == 0)
			sphere8->render(view * translate(pelletLoc3[8]) * scale(0.5f, 0.5f, 0.5f), projection);
		if (renderPellets[9] == 0)
			sphere9->render(view * translate(pelletLoc3[9]) * scale(0.5f, 0.5f, 0.5f), projection);

		freezeCharacter("pacman");
		freezeCharacter("ghost1");
		freezeCharacter("ghost2");
		freezeCharacter("ghost3");
		freezeCharacter("ghost4");

		pacManXPos = 0.0f;
		pacManYPos = 0.0f;
		ghost1XPos = -11.3403f;
		ghost1YPos = 10.1f;
		ghost2XPos = 11.3403f;
		ghost2YPos = 10.1f;
		ghost3XPos = -11.3403f;
		ghost3YPos = -10.1f;
		ghost4XPos = 11.3403f;
		ghost4YPos = -10.1f;

		for (int i = 1; i < NUMPELLETS; i++) {
			renderPellets[i] = 1;
		}
		score = 0;




	}


		glColor3f(0.0, 1.0, 0.0);
		glRasterPos2f(-0.97f, 0.9f);

		stringstream strs;
		strs << score;
		std::string temp = strs.str();
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
		char* title = "Game Over";
		if (gameOverInt == 1000) {
			title = "YOU WIN!!!";
		}
		for (c = title; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		glColor3f(1.0, 1.0, 0.0);
		glRasterPos2f(-0.17f, 0.75f);
		char* title2 = "Press 'r' to restart!";
		for (c = title2; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}

		//check if ghost touches pacman
		glColor3f(0.0, 1.0, 0.0);
		glRasterPos2f(0.55f, 0.9f);
		stringstream strs1;
		strs1 << gameOverInt;
		temp = strs1.str();
		scr_temp = (char*)temp.c_str();
		string = "Time in Contact w/ Ghost: ";
		i = 5;
		for (c = string; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		}
		for (c = scr_temp; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		}
		//current dist ghost is from pacman
		glColor3f(0.0, 1.0, 0.0);
		glRasterPos2f(0.7f, 0.85f);
		stringstream strs2;
		strs2 << ghost3NextDir;
		temp = strs2.str();
		scr_temp = (char*)temp.c_str();
		string = "ghost1NextDir: ";
		i = 5;
		//for (c = string; *c != '\0'; c++) {
		//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		//}
		//for (c = scr_temp; *c != '\0'; c++) {
		//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
		//}
		glutSwapBuffers(); // Swap the buffers.
		checkError("display");

		pacMan->render(view* translate(pelletLoc3[5])* rotate(pacManRot, 0.0f, 0.0f, 1.0f), projection);

		redGhost->render(view* translate(pelletLoc3[1])* rotate(ghost1Rot, 0.0f, 0.0f, 1.0f), projection);
		cyanGhost->render(view* translate(pelletLoc3[3])* rotate(ghost2Rot, 0.0f, 0.0f, 1.0f), projection);
		orangeGhost->render(view* translate(pelletLoc3[7])* rotate(ghost3Rot, 0.0f, 0.0f, 1.0f), projection);
		pinkGhost->render(view* translate(pelletLoc3[9])* rotate(ghost4Rot, 0.0f, 0.0f, 1.0f), projection);

		for (int i = 1; i < NUMPELLETS; i++) {
			renderPellets[i] = 0;
		}
		score = 0;
		freezeCharacter("pacman");
		freezeCharacter("ghost1");
		freezeCharacter("ghost2");
		freezeCharacter("ghost3");
		freezeCharacter("ghost4");
	}

	
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
	case 'r':
		if (gameOverInt>199) {
			gameOverInt=0;
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

	redGhost = new Model(&redShader, "models/ghost.obj", "models/");
	cyanGhost = new Model(&cyanShader, "models/ghost.obj", "models/");
	orangeGhost = new Model(&orangeShader, "models/ghost.obj", "models/");
	pinkGhost = new Model(&pinkShader, "models/ghost.obj", "models/");

	//maze0 = new Model(&blueShader, "models/maze0.obj", "models/");

	maze0 = new Model(&shader, "models/maze0.obj", "models/");

	sphere1 = new Model(&yellowShader, "models/sphere.obj");
	sphere2 = new Model(&yellowShader, "models/sphere.obj");
	sphere3 = new Model(&yellowShader, "models/sphere.obj");
	sphere4 = new Model(&yellowShader, "models/sphere.obj");
	sphere5 = new Model(&yellowShader, "models/sphere.obj");
	sphere6 = new Model(&yellowShader, "models/sphere.obj");
	sphere7 = new Model(&yellowShader, "models/sphere.obj");
	sphere8 = new Model(&yellowShader, "models/sphere.obj");
	sphere9 = new Model(&yellowShader, "models/sphere.obj");

	cylinder = new Model(&shader, "models/cylinder.obj", "models/");

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

	pelletLoc3[1] = vec3(-11.3403f, 10.1f, OBJ_DEPTH);
	pelletLoc3[2] = vec3(0.0f, 10.1f, OBJ_DEPTH);
	pelletLoc3[3] = vec3(11.3403f, 10.1f, OBJ_DEPTH);
	pelletLoc3[4] = vec3(-11.3403f, 0.0f, OBJ_DEPTH);
	pelletLoc3[5] = vec3(0.0f, 0.0f, OBJ_DEPTH);
	pelletLoc3[6] = vec3(11.3403f, 0.0f, OBJ_DEPTH);
	pelletLoc3[7] = vec3(-11.3403f, -10.1f, OBJ_DEPTH);
	pelletLoc3[8] = vec3(0.0f, -10.1f, OBJ_DEPTH);
	pelletLoc3[9] = vec3(11.3403f, -10.1f, OBJ_DEPTH);

	glutMainLoop();

	return 0;
}