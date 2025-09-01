#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shape_class.hpp"


#include "gl_framework.hpp"

class shape_t;
namespace csX75	 { 

	// A simple class that represents a node in the hierarchy tree
	class HNode {
		//glm::vec4 * vertices;
		//glm::vec4 * colors;
		GLfloat tx,ty,tz,rx,ry,rz;
		GLfloat sx = 1.0;
		GLfloat sy = 1.0;
		GLfloat sz = 1.0;


		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;

		GLuint num_vertices;
		GLuint vao,vbo;

		glm::mat4 rotation;
		glm::mat4 translation;
		glm::mat4 scaling;
		
		

		void update_matrices();

	  public:
	    shape_t* shape_pointer;
		HNode (HNode*, GLuint, glm::vec4*,  glm::vec4*, std::size_t, std::size_t);
		//HNode (HNode* , glm::vec4*,  glm::vec4*,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		std::vector<HNode*> children;
		HNode* parent;
		void add_child(HNode*);
		void render();
		void change_parameters(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void render_tree();
		void inc();
		void dec();
		void delete_node();
		void addShape(int shape_type);
		void save(const std::string& filename);
		void save_tree(std::ofstream& outFile);
		void load(const std::string& filename);
	};

	glm::mat4* multiply_stack(std::vector <glm::mat4> );
};	

#endif