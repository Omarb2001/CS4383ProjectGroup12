#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "tiny_obj_loader.h"
#include <string>
#include "Shader.h"

using namespace glm;

class Intersection {
public:
	vec3 position; /// x,y,z 
	std::vector<std::string> directions;

	Intersection(vec3 position, std::vector<std::string> directions);
	~Intersection(void) {} // default destructor

private:

};



