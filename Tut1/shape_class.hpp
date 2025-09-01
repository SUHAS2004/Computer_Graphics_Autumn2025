#ifndef _SHAPE_CLASS_HPP_
#define _SHAPE_CLASS_HPP_

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"

enum shape_type {SPHERE_SHAPE, CYLINDER_SHAPE, BOX_SHAPE, CONE_SHAPE};

class shape_t {
    public:
        explicit shape_t(unsigned int level);
        virtual ~shape_t();

        std::vector <glm::vec4> vertices;
        std::vector <glm::vec4> colors;
        int num_vertices;
        GLfloat position[3];
        GLfloat rotation[3];
        GLfloat scaling[3]; 
        shape_type shape;
        int shape_int;

        unsigned int level;
        virtual void draw()=0;
       
};

class sphere_t : public shape_t {
    public:
        ~sphere_t() override = default;
        void draw() override;
        explicit sphere_t(unsigned int level);


};
class cylinder_t : public shape_t {
    public:
        ~cylinder_t() override = default;
        void draw() override;
        explicit cylinder_t(unsigned int level);
        

};
class cone_t : public shape_t {
    public:
        ~cone_t() override = default;
        void draw() override;
        explicit cone_t(unsigned int level);
        

};

#endif