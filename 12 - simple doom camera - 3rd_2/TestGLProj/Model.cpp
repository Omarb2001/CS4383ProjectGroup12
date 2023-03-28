#include "Model.h"


Model::Model(Shader* shader, const char* filename, const char* materialPath)
{
	m_shader = shader;
	tinyobj::LoadObj(shapes, filename, materialPath);
	for (int i = 0; i < shapes.size(); i++) {
		m_VBO.push_back(0);
		m_NBO.push_back(0);
		m_IBO.push_back(0);
		glGenBuffers(1, &m_VBO[i]); // generate a 'name' for the VBO
		glGenBuffers(1, &m_NBO[i]); // generate a 'name' for the NBO
		glGenBuffers(1, &m_IBO[i]); // generate a 'name' for the IBO
		// Bind ibo to the index buffer.
	}


	// Torso
	vboT = 0;
	nboT = 0;
	iboT = 0;
	glGenBuffers(1, &vboT); // generate a 'name' for the VBO
	glGenBuffers(1, &nboT); // generate a 'name' for the NBO
	glGenBuffers(1, &iboT); // generate a 'name' for the IBO

	float maxX = 1.5; float minX = -1.5;
	float maxY = 2; float minY = -2;
	float maxZ = 2; float minZ = 0;

	vertexesT = { minX, minY, minZ, //0
		minX, minY, maxZ, //1
		maxX, minY, maxZ, //2
		maxX, minY, minZ, //3
		minX, maxY, minZ, //4
		minX, maxY, maxZ, //5
		maxX, maxY, maxZ, //6
		maxX, maxY, minZ //7
	};

	normalsT = { 0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};



	indicesT = { 5,1,2,   5,6,2, //square 1
		5,1,0,  5,4,0, //Square 2
		6,2,3,  6,7,3, //Square 3
		7,4,5,  7,6,5, //Square 4
		1,0,3,  1,2,3, //Square 5
		4,0,3,  4,7,3  //Square 6
	};

	//Head
	vboH = 0;
	nboH = 0;
	iboH = 0;
	glGenBuffers(1, &vboH); // generate a 'name' for the VBO
	glGenBuffers(1, &nboH); // generate a 'name' for the NBO
	glGenBuffers(1, &iboH); // generate a 'name' for the IBO

	maxX = 0.9; minX = -0.9;
	maxY = 3.5; minY = 2;
	maxZ = 1.75; minZ = 0.25;

	vertexesH = { minX, minY, minZ, //0
		minX, minY, maxZ, //1
		maxX, minY, maxZ, //2
		maxX, minY, minZ, //3
		minX, maxY, minZ, //4
		minX, maxY, maxZ, //5
		maxX, maxY, maxZ, //6
		maxX, maxY, minZ //7
	};

	normalsH = { 0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};



	indicesH = { 5,1,2,   5,6,2, //square 1
		5,1,0,  5,4,0, //Square 2
		6,2,3,  6,7,3, //Square 3
		7,4,5,  7,6,5, //Square 4
		1,0,3,  1,2,3, //Square 5
		4,0,3,  4,7,3  //Square 6
	};

	//Right Arm
	vboRA = 0;
	nboRA = 0;
	iboRA = 0;
	glGenBuffers(1, &vboRA); // generate a 'name' for the VBO
	glGenBuffers(1, &nboRA); // generate a 'name' for the NBO
	glGenBuffers(1, &iboRA); // generate a 'name' for the IBO

	maxX = 2.45; minX = 1.6;
	maxY = 1.95; minY = -2.5;
	maxZ = 1.25; minZ = 0.5;

	vertexesRA = { minX, minY, minZ, //0
		minX, minY, maxZ, //1
		maxX, minY, maxZ, //2
		maxX, minY, minZ, //3
		minX, maxY, minZ, //4
		minX, maxY, maxZ, //5
		maxX, maxY, maxZ, //6
		maxX, maxY, minZ //7
	};

	normalsRA = { 0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};



	indicesRA = { 5,1,2,   5,6,2, //square 1
		5,1,0,  5,4,0, //Square 2
		6,2,3,  6,7,3, //Square 3
		7,4,5,  7,6,5, //Square 4
		1,0,3,  1,2,3, //Square 5
		4,0,3,  4,7,3  //Square 6
	};

	//Left Arm
	vboLA = 0;
	nboLA = 0;
	iboLA = 0;
	glGenBuffers(1, &vboLA); // generate a 'name' for the VBO
	glGenBuffers(1, &nboLA); // generate a 'name' for the NBO
	glGenBuffers(1, &iboLA); // generate a 'name' for the IBO

	maxX = -1.6; minX = -2.45;
	maxY = 1.95; minY = -2.5;
	maxZ = 1.25; minZ = 0.5;

	vertexesLA = { minX, minY, minZ, //0
		minX, minY, maxZ, //1
		maxX, minY, maxZ, //2
		maxX, minY, minZ, //3
		minX, maxY, minZ, //4
		minX, maxY, maxZ, //5
		maxX, maxY, maxZ, //6
		maxX, maxY, minZ //7
	};

	normalsLA = { 0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};



	indicesLA = { 5,1,2,   5,6,2, //square 1
		5,1,0,  5,4,0, //Square 2
		6,2,3,  6,7,3, //Square 3
		7,4,5,  7,6,5, //Square 4
		1,0,3,  1,2,3, //Square 5
		4,0,3,  4,7,3  //Square 6
	};

	//Right Leg
	vboRL = 0;
	nboRL = 0;
	iboRL = 0;
	glGenBuffers(1, &vboRL); // generate a 'name' for the VBO
	glGenBuffers(1, &nboRL); // generate a 'name' for the NBO
	glGenBuffers(1, &iboRL); // generate a 'name' for the IBO

	maxX = 1.25; minX = 0.5;
	maxY = -2.1; minY = -6;
	maxZ = 1.25; minZ = 0.5;

	vertexesRL = { minX, minY, minZ, //0
		minX, minY, maxZ, //1
		maxX, minY, maxZ, //2
		maxX, minY, minZ, //3
		minX, maxY, minZ, //4
		minX, maxY, maxZ, //5
		maxX, maxY, maxZ, //6
		maxX, maxY, minZ //7
	};

	normalsRL = { 0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};



	indicesRL = { 5,1,2,   5,6,2, //square 1
		5,1,0,  5,4,0, //Square 2
		6,2,3,  6,7,3, //Square 3
		7,4,5,  7,6,5, //Square 4
		1,0,3,  1,2,3, //Square 5
		4,0,3,  4,7,3  //Square 6
	};

	//Left Leg
	vboLL = 0;
	nboLL = 0;
	iboLL = 0;
	glGenBuffers(1, &vboLL); // generate a 'name' for the VBO
	glGenBuffers(1, &nboLL); // generate a 'name' for the NBO
	glGenBuffers(1, &iboLL); // generate a 'name' for the IBO

	maxX = -0.5;  minX = -1.25;
	maxY = -2.1; minY = -6;
	maxZ = 1.25; minZ = 0.5;

	vertexesLL = { minX, minY, minZ, //0
		minX, minY, maxZ, //1
		maxX, minY, maxZ, //2
		maxX, minY, minZ, //3
		minX, maxY, minZ, //4
		minX, maxY, maxZ, //5
		maxX, maxY, maxZ, //6
		maxX, maxY, minZ //7
	};

	normalsLL = { 0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};



	indicesLL = { 5,1,2,   5,6,2, //square 1
		5,1,0,  5,4,0, //Square 2
		6,2,3,  6,7,3, //Square 3
		7,4,5,  7,6,5, //Square 4
		1,0,3,  1,2,3, //Square 5
		4,0,3,  4,7,3  //Square 6
	};
	updateBuffers();

}


/*
Render the mesh to the screen
@param Modelview - the model view matrix that defines where the camera is looking
@param Projection - the projection matrix that defines how 3D vertices are projected on the 2D screen.
*/
void Model::render(glm::mat4 ModelView, glm::mat4 Projection) {

	m_shader->Activate(); // Bind shader.

	//update the variables in the shader program
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader
	m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader
	for (int i = 0; i < shapes.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]); // Bind VBO.
		glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
		glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); // Attribute pointer.
		glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]); // Bind NBO.
		glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
		glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]); // Bind IBO.
		//glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, 0); // Draw using indices
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboT); // Bind VBO. //Torso
	glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
	glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, nboT); // Bind NBO.
	glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
	glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboT); // Bind IBO.
	glDrawElements(GL_TRIANGLES, indicesT.size(), GL_UNSIGNED_INT, 0);
	m_shader->DeActivate(); // Unbind shader. 
}
void Model::renderPlane(glm::mat4 ModelView, glm::mat4 Projection) {

	m_shader->Activate(); // Bind shader.

	//update the variables in the shader program
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader
	m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader
	for (int i = 0; i < shapes.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]); // Bind VBO.
		glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
		glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); // Attribute pointer.
		glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]); // Bind NBO.
		glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
		glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]); // Bind IBO.
		glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, 0); // Draw using indices
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboT); // Bind VBO. //Torso
	glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
	glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, nboT); // Bind NBO.
	glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
	glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboT); // Bind IBO.
	//glDrawElements(GL_TRIANGLES, indicesT.size(), GL_UNSIGNED_INT, 0);
	m_shader->DeActivate(); // Unbind shader. 
}

void Model::renderHead(glm::mat4 ModelView, glm::mat4 Projection) {
	m_shader->Activate();
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader
	m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, vboH); // Bind VBO. //Head
	glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
	glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, nboH); // Bind NBO.
	glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
	glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboH); // Bind IBO.
	glDrawElements(GL_TRIANGLES, indicesH.size(), GL_UNSIGNED_INT, 0);

	m_shader->DeActivate();
}

void Model::renderRA(glm::mat4 ModelView, glm::mat4 Projection) {
	m_shader->Activate();
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader
	m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, vboRA); // Bind VBO. //Right Arm
	glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
	glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, nboRA); // Bind NBO.
	glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
	glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboRA); // Bind IBO.
	glDrawElements(GL_TRIANGLES, indicesRA.size(), GL_UNSIGNED_INT, 0);

	m_shader->DeActivate();
}

void Model::renderLA(glm::mat4 ModelView, glm::mat4 Projection) {
	m_shader->Activate();
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader
	m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, vboLA); // Bind VBO. //Left Arm
	glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
	glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, nboLA); // Bind NBO.
	glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
	glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboLA); // Bind IBO.
	glDrawElements(GL_TRIANGLES, indicesLA.size(), GL_UNSIGNED_INT, 0);

	m_shader->DeActivate();
}

void Model::renderRL(glm::mat4 ModelView, glm::mat4 Projection) {
	m_shader->Activate();
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader
	m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, vboRL); // Bind VBO. //Right Leg
	glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
	glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, nboRL); // Bind NBO.
	glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
	glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboRL); // Bind IBO.
	glDrawElements(GL_TRIANGLES, indicesRL.size(), GL_UNSIGNED_INT, 0);

	m_shader->DeActivate();
}

void Model::renderLL(glm::mat4 ModelView, glm::mat4 Projection) {
	m_shader->Activate();
	m_shader->SetUniform("Projection", Projection); // send projection to vertex shader
	m_shader->SetUniform("ModelView", ModelView);  // send modelview to vertex shader
	m_shader->SetUniform("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0)); // send light position to vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, vboLL); // Bind VBO. //Left Leg
	glEnableVertexAttribArray((*m_shader)["vertexPosition"]); // Enable vertex attribute.
	glVertexAttribPointer((*m_shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, nboLL); // Bind NBO.
	glEnableVertexAttribArray((*m_shader)["vertexNormal"]); // Enable normal attribute.
	glVertexAttribPointer((*m_shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboLL); // Bind IBO.
	glDrawElements(GL_TRIANGLES, indicesLL.size(), GL_UNSIGNED_INT, 0);

	m_shader->DeActivate();
}
/*When a new vertex is added, you need to tell the VBO about it.*/
void Model::updateBuffers()
{
	for (int i = 0; i < shapes.size(); i++) {
		// Tell OpenGL which VBO you want to work on right now
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
		// Fillthe VBO with vertex data.
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float), &shapes[i].mesh.positions[0], GL_STATIC_DRAW);
		// Tell OpenGL which VBO you want to work on right now
		glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]);
		// Fillthe VBO with vertex data.
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.normals.size() * sizeof(float), &shapes[i].mesh.normals[0], GL_STATIC_DRAW);
		// Bind ibo to the index buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]);
		// Fill index buffer with index data.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), &shapes[i].mesh.indices[0], GL_STATIC_DRAW);
	}

	// Tell OpenGL which VBO you want to work on right now //Torso
	glBindBuffer(GL_ARRAY_BUFFER, vboT);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, vertexesT.size() * sizeof(float), &vertexesT[0], GL_STATIC_DRAW);
	// Tell OpenGL which VBO you want to work on right now
	glBindBuffer(GL_ARRAY_BUFFER, nboT);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, normalsT.size() * sizeof(float), &normalsT[0], GL_STATIC_DRAW);
	// Bind ibo to the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboT);
	// Fill index buffer with index data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesT.size() * sizeof(unsigned int), &indicesT[0], GL_STATIC_DRAW);

	// Tell OpenGL which VBO you want to work on right now //Head
	glBindBuffer(GL_ARRAY_BUFFER, vboH);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, vertexesH.size() * sizeof(float), &vertexesH[0], GL_STATIC_DRAW);
	// Tell OpenGL which VBO you want to work on right now
	glBindBuffer(GL_ARRAY_BUFFER, nboH);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, normalsH.size() * sizeof(float), &normalsH[0], GL_STATIC_DRAW);
	// Bind ibo to the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboH);
	// Fill index buffer with index data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesH.size() * sizeof(unsigned int), &indicesH[0], GL_STATIC_DRAW);

	// Tell OpenGL which VBO you want to work on right now //Right Arm
	glBindBuffer(GL_ARRAY_BUFFER, vboRA);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, vertexesRA.size() * sizeof(float), &vertexesRA[0], GL_STATIC_DRAW);
	// Tell OpenGL which VBO you want to work on right now
	glBindBuffer(GL_ARRAY_BUFFER, nboRA);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, normalsRA.size() * sizeof(float), &normalsRA[0], GL_STATIC_DRAW);
	// Bind ibo to the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboRA);
	// Fill index buffer with index data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesRA.size() * sizeof(unsigned int), &indicesRA[0], GL_STATIC_DRAW);

	// Tell OpenGL which VBO you want to work on right now //Left Arm
	glBindBuffer(GL_ARRAY_BUFFER, vboLA);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, vertexesLA.size() * sizeof(float), &vertexesLA[0], GL_STATIC_DRAW);
	// Tell OpenGL which VBO you want to work on right now
	glBindBuffer(GL_ARRAY_BUFFER, nboLA);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, normalsLA.size() * sizeof(float), &normalsLA[0], GL_STATIC_DRAW);
	// Bind ibo to the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboLA);
	// Fill index buffer with index data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLA.size() * sizeof(unsigned int), &indicesLA[0], GL_STATIC_DRAW);

	// Tell OpenGL which VBO you want to work on right now //Right Leg
	glBindBuffer(GL_ARRAY_BUFFER, vboRL);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, vertexesRL.size() * sizeof(float), &vertexesRL[0], GL_STATIC_DRAW);
	// Tell OpenGL which VBO you want to work on right now
	glBindBuffer(GL_ARRAY_BUFFER, nboRL);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, normalsRL.size() * sizeof(float), &normalsRL[0], GL_STATIC_DRAW);
	// Bind ibo to the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboRL);
	// Fill index buffer with index data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesRL.size() * sizeof(unsigned int), &indicesRL[0], GL_STATIC_DRAW);

	// Tell OpenGL which VBO you want to work on right now //Left Leg
	glBindBuffer(GL_ARRAY_BUFFER, vboLL);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, vertexesLL.size() * sizeof(float), &vertexesLL[0], GL_STATIC_DRAW);
	// Tell OpenGL which VBO you want to work on right now
	glBindBuffer(GL_ARRAY_BUFFER, nboLL);
	// Fillthe VBO with vertex data.
	glBufferData(GL_ARRAY_BUFFER, normalsLL.size() * sizeof(float), &normalsLL[0], GL_STATIC_DRAW);
	// Bind ibo to the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboLL);
	// Fill index buffer with index data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLL.size() * sizeof(unsigned int), &indicesLL[0], GL_STATIC_DRAW);

}
