#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "tiny_obj_loader.h"
#include <string>
#include "Shader.h"

/*This class represents a mesh - a collection of triangles. It stores a list of vertices, which are interpreted by openGL as every 3 vertices makes a triangle.*/
class Model {

public:

	/*This struct represents a vertex in 3D space*/
	struct Vertex
	{
		Vertex() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f) {} // default values
		glm::vec3 Position; /// x,y,z 
		glm::vec3 Normal; // determines which way the vertex is 'facing'

	};

	/*
	@param shader - a pointer to the shader program to use
	@param filename - the name of the file with the relative directory included
	@param materialPath - [optional] the relative path to the mtl files. NOTE: if your obj has a mtl file associated with it, this is not optional!
	*/
	Model(Shader* shader, const char* filename, const char* materialPath = NULL);
	~Model(void) {} // default destructor
	void render(glm::mat4 ModelView, glm::mat4 Projection);
	void renderPlane(glm::mat4 ModelView, glm::mat4 Projection);
	void renderHead(glm::mat4 ModelView, glm::mat4 Projection);// render the model
	void renderRA(glm::mat4 ModelView, glm::mat4 Projection);
	void renderLA(glm::mat4 ModelView, glm::mat4 Projection);
	void renderRL(glm::mat4 ModelView, glm::mat4 Projection);
	void renderLL(glm::mat4 ModelView, glm::mat4 Projection);

private:
	Shader* m_shader; // shader program
	std::vector<tinyobj::shape_t> shapes; //a list of meshes and their respective materials
	std::vector<GLuint> m_VBO;// vertex buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_NBO;// normal buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_IBO;// index buffer IDs, each corresponding to a shape

	void updateBuffers(); //initialize your VBO and update when triangles are added

	//torso
	GLuint vboT, nboT, iboT;
	std::vector<float> vertexesT;
	std::vector<float> normalsT;
	std::vector<unsigned int> indicesT;


	//Head
	GLuint vboH, nboH, iboH;
	std::vector<float> vertexesH;
	std::vector<float> normalsH;
	std::vector<unsigned int> indicesH;

	//Right Arm
	GLuint vboRA, nboRA, iboRA;
	std::vector<float> vertexesRA;
	std::vector<float> normalsRA;
	std::vector<unsigned int> indicesRA;

	//Left Arm
	GLuint vboLA, nboLA, iboLA;
	std::vector<float> vertexesLA;
	std::vector<float> normalsLA;
	std::vector<unsigned int> indicesLA;

	//Right Leg
	GLuint vboRL, nboRL, iboRL;
	std::vector<float> vertexesRL;
	std::vector<float> normalsRL;
	std::vector<unsigned int> indicesRL;

	//Left Leg
	GLuint vboLL, nboLL, iboLL;
	std::vector<float> vertexesLL;
	std::vector<float> normalsLL;
	std::vector<unsigned int> indicesLL;


};