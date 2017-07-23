//
//  DynamicMesh.h
//  BobosPuzzle
//
//  Created by Jas S on 2017-05-09.
//  Copyright © 2017 Jas S. All rights reserved.
//

#ifndef DynamicMesh_h
#define DynamicMesh_h

#include <stdio.h>

#include <map>
#include <vector>

#include "GL.h"
#include "Bone.h"
#include "Node.h"
#include "Mesh.h"
#include "Math3d.h"
#include "Texture.h"
#include "Transform.h"
#include "ColladaLoader.h"

#define DYNAMIC_MESH_MAX_TEXTURE_COUNT 4
#define DYNAMIC_MESH_MAX_BONE_COUNT 16
#define DYNAMIC_MESH_MAX_NODE_COUNT 16

class DynamicMesh : public Mesh
{
protected:
    Texture* textures;
    unsigned int texture_count;
    
    glm::mat4 inverse_root;
    
    std::vector<Bone> bones;
    std::vector<Node> nodes;
    std::map<std::string, Bone*> bone_map;
    std::map<std::string, Node*> node_map;
    
    void generateNodes(const ColladaLoader& loader);
    void construct(const char* path, const glm::vec3& _scale = glm::vec3(1.0f), GLenum draw_mode = GL_STATIC_DRAW) { this->construct(ColladaLoader(path), _scale, draw_mode); }
    void construct(const ColladaLoader& loader, const glm::vec3& _scale = glm::vec3(1.0f), GLenum draw_mode = GL_STATIC_DRAW);
    
public:
    DynamicMesh(){}
    DynamicMesh(const char* path, const glm::vec3& _scale = glm::vec3(1.0f), GLenum draw_mode = GL_STATIC_DRAW) { this->construct(path, _scale, draw_mode); }
    void destroy();
    
    inline const Texture* getTextures() const { return this->textures; }
    inline unsigned int getTextureCount() const { return this->texture_count; }
    
    inline const glm::mat4& getInverseRoot() const { return this->inverse_root; }
    inline const std::vector<Bone>& getBones() const { return this->bones; }
    inline const std::vector<Node>& getNodes() const { return this->nodes; }
    inline Bone* getBone(const std::string& str) const { return bone_map.at(str); }
};

#endif /* DynamicMesh_h */
