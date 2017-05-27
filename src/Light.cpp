#include "Light.h"

//
// PointLight
// 

GLuint Light::PointLight::_vao;
GLuint Light::PointLight::_vbo;
unsigned int Light::PointLight::_vertex_count;

Light::PointLight::PointLight(const glm::vec3& _position, const glm::vec3& _size, const glm::vec3& _color)
{
	this->position = _position;
	this->size = _size;
	this->color = _color;
}

void Light::PointLight::_init_system()
{
    #ifdef _WIN32
        const char* LIGHT_SPHERE_PATH = "C:\\Users\\Jas\\Documents\\GameKit\\src\\Data\\UnitSphere.obj";
    #else
        const char* LIGHT_SPHERE_PATH = "/Users/Jas/Documents/GameKit/src/Data/UnitSphere.obj";
    #endif
    
	OBJ_Loader loader = OBJ_Loader(LIGHT_SPHERE_PATH, OBJ_Loader::VN);
	std::vector<float> vertex_buffer;
	vertex_buffer.reserve(loader.getIndices().size() * 3);
	for (unsigned int i = 0; i < loader.getIndices().size(); i++)
	{
		const float* v = &loader.getVertices().at(loader.getIndices().at(i).x - 1)[0];
		vertex_buffer.insert(vertex_buffer.end(), v, v + 3);
	}

	_vertex_count = (unsigned int) vertex_buffer.size() / 3;
	
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_buffer.size(), &vertex_buffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Light::PointLight::_free_system()
{
	if (glIsVertexArray(_vao) == GL_TRUE) glDeleteVertexArrays(1, &_vao);
	if (glIsBuffer(_vbo) == GL_TRUE) glDeleteBuffers(1, &_vbo);
}

//
// Light
//

std::vector<Light::PointLight> Light::point_lights;

void Light::init_system()
{
	PointLight::_init_system();
}

void Light::insert(const PointLight& point_light)
{
	point_lights.push_back(point_light);
}

void Light::free_system()
{
	PointLight::_free_system();
}