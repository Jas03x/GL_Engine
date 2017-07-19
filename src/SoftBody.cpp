//
//  SoftBody.cpp
//  CarDemo
//
//  Created by Jas S on 2017-07-01.
//  Copyright © 2017 Jas S. All rights reserved.
//

#include "SoftBody.h"

SoftBody::SoftBody(const ColliderConfiguration& collider_configuration, ColladaLoader& loader, const std::vector<int>& face_data, const Transform& transform, const glm::vec3& _scale, Transform* ptr) : Collider(ptr)
{
    std::vector<glm::mat4> bone_cache;
    bone_cache.reserve(loader.getBoneNames().size());
    for(unsigned int i = 0; i < loader.getBoneNames().size(); i++) {
        const std::string& name = loader.getBoneNames()[i];
        const glm::mat4& bindpose = loader.getNodeTransforms().at(name);
        const glm::mat4& offset = loader.getBoneOffsets().at(name);
        bone_cache.push_back(loader.getInverseRoot() * bindpose * offset);
    }
    const glm::mat4 scale_matrix = glm::scale(glm::vec3(_scale.x, _scale.y, -_scale.z));
    
    this->face_array = face_data;
    this->face_array.shrink_to_fit();
    
    std::vector<glm::vec3> vertex_array;
    vertex_array.reserve(face_data.size());
    for(unsigned int i = 0; i < face_data.size(); i++)
    {
        const glm::vec3& vertex = loader.getVertices()[face_array[i]];
        const glm::uvec4& index = loader.getBoneIndices()[face_array[i]];
        const glm::vec4& weight = loader.getBoneWeights()[face_array[i]];
        glm::mat4 joint =
            weight.x * bone_cache[index.x] +
            weight.y * bone_cache[index.y] +
            weight.z * bone_cache[index.z] +
            weight.w * bone_cache[index.w];
        glm::vec3 v = glm::vec3(scale_matrix * joint * glm::vec4(vertex, 1));
        vertex_array.push_back(v);
    }
    
    this->scale = glm::vec3(1.0f / _scale.x, 1.0f / _scale.y, -1.0f / _scale.z);
    this->body = btSoftBodyHelpers::CreateFromTriMesh(PhysicsConfiguration::softbody_info, &vertex_array[0][0], &this->face_array[0], (unsigned int) this->face_array.size() / 3);
    this->bind();
    
    // finally update the loader
    loader.removeVertexBones(face_data);
}

SoftBody::~SoftBody()
{
    this->unbind();
}

void SoftBody::bind()
{
    PhysicsConfiguration::addSoftBody(this);
}

void SoftBody::unbind()
{
    PhysicsConfiguration::removeSoftBody(this);
}

void SoftBody::getVertexData(std::vector<glm::vec3>& vertex_data)
{
    btSoftBody::tNodeArray& nodes = ((btSoftBody*) body)->m_nodes;
    vertex_data.clear();
    vertex_data.reserve(this->face_array.size());
    
    for(unsigned int i = 0; i < this->face_array.size(); i++)
    {
        const btVector3& x = nodes[this->face_array[i]].m_x;
        glm::vec3 vec = glm::vec3(x.getX(), x.getY(), x.getZ());
        vertex_data.push_back(glm::vec3(glm::scale(this->scale) * glm::vec4(vec, 1)));
    }
}


// OLD FUNCTION FOR SKINNED SOFT BODIES:
// VERY SLOW - NOT GOING TO BE USED FOR SOFT BODIES
// PLUS OPTIMIZATIONS SUCH AS COMPUTE SHADERS AND CACHING WILL PROBABLY OVER COMPLICATE THIS CODE BASE
/*
void SoftBody::getVertexData(std::vector<glm::vec3>& vertex_data)
{
    btSoftBody::tNodeArray& nodes = ((btSoftBody*) body)->m_nodes;
    vertex_data.clear();
    vertex_data.reserve(this->face_array.size());
    
    for(unsigned int i = 0; i < this->face_array.size(); i++)
    {
        const btVector3& x = nodes[this->face_array[i]].m_x;
        glm::vec3 vec = glm::vec3(x.getX(), x.getY(), x.getZ());
        const glm::uvec4& index = this->data->getBoneIndices()[face_array[i]];
        const glm::vec4& weight = this->data->getBoneWeights()[face_array[i]];
        glm::mat4 joint =
            weight.x * this->bone_cache[index.x] +
            weight.y * this->bone_cache[index.y] +
            weight.z * this->bone_cache[index.z] +
            weight.w * this->bone_cache[index.w];
        vertex_data.push_back(glm::vec3(glm::inverse(joint) * glm::scale(this->scale) * glm::vec4(vec, 1)));
    }
}
 */
