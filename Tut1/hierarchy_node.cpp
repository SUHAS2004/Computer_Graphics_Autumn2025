#include "hierarchy_node.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>

extern GLuint vPosition,vColor,uModelViewMatrix;
extern csX75::HNode *curr_node;
extern std::vector<glm::mat4> matrixStack;
extern float delta_factor;

namespace csX75
{

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours, std::size_t v_size, std::size_t c_size){

		num_vertices = num_v;
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		// initialize vao and vbo of the object;

		std::vector<HNode*> children;
		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));


		// set parent
		
		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=0;

		update_matrices();
	}

	void HNode::update_matrices(){

		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(8*rx), glm::vec3(1.0f,0.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(8*ry), glm::vec3(0.0f,1.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(8*rz), glm::vec3(0.0f,0.0f,1.0f));

		translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));

		scaling = glm::scale(glm::mat4(1.0f),glm::vec3(sx,sy,sz));


	}

	void HNode::add_child(HNode* a_child){
		children.push_back(a_child);

	}

	void HNode::delete_node(){
		parent->children.pop_back();
	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz, GLfloat asx, GLfloat asy, GLfloat asz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz; 
        sx = asx;
        sy = asy;
        sz = asz;

		update_matrices();
	}

	void HNode::render(){

		//matrixStack multiply
		glm::mat4* ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		glBindVertexArray (vao);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		// for memory 
		delete ms_mult;

	}

	void HNode::render_tree(){
		
		matrixStack.push_back(translation);
		matrixStack.push_back(rotation);
		matrixStack.push_back(scaling);

		render();
		for(int i=0;i<children.size();i++){
			children[i]->render_tree();
		}
		matrixStack.pop_back();
		matrixStack.pop_back();

	}

	void HNode::inc(){
		if(CurrentMode == ROTATE){
			if(CurrentAxis == X_AXIS)
				rx += delta_factor;
			if(CurrentAxis == Y_AXIS)
				ry += delta_factor;
			if(CurrentAxis == Z_AXIS)
				rz += delta_factor;
		}
		else if(CurrentMode == TRANSLATE){
			if(CurrentAxis == X_AXIS)
				tx += delta_factor;
			if(CurrentAxis == Y_AXIS)
				ty += delta_factor;
			if(CurrentAxis == Z_AXIS)
				tz += delta_factor;
		}
		else if(CurrentMode == SCALE){
			if(CurrentAxis == X_AXIS)
				sx += delta_factor;
			if(CurrentAxis == Y_AXIS)
				sy += delta_factor;
			if(CurrentAxis == Z_AXIS)
				sz += delta_factor;
		}
		update_matrices();
	}

	void HNode::dec(){
		if(CurrentMode == ROTATE){
			if(CurrentAxis == X_AXIS)
				rx = rx - delta_factor;
			if(CurrentAxis == Y_AXIS)
				ry = ry - delta_factor;
			if(CurrentAxis == Z_AXIS)
				rz = rz - delta_factor;
		}
		else if(CurrentMode == TRANSLATE){
			if(CurrentAxis == X_AXIS)
				tx = tx - delta_factor;
			if(CurrentAxis == Y_AXIS)
				ty = ty - delta_factor;
			if(CurrentAxis == Z_AXIS)
				tz = tz - delta_factor;
		}
		else if(CurrentMode == SCALE){
			if(CurrentAxis == X_AXIS)
				sx = sx - delta_factor;
			if(CurrentAxis == Y_AXIS)
				sy = sy - delta_factor;
			if(CurrentAxis == Z_AXIS)
				sz = sz - delta_factor;
		}
		update_matrices();
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}


    void HNode::addShape(int shape_type){ // shape type is made back to zero in renderGL function
        std::cout << "adding shape" << shape_type << "\n";
        const int num_vertices = 10000;
        glm::vec4 v_positions[num_vertices];
        glm::vec4 v_colors[num_vertices];
        int initial_level = 3;
        shape_t* new_shape = nullptr;
        if(shape_type == 1){
            new_shape = new cone_t(initial_level);
            new_shape->draw();
        }
        else if(shape_type == 2){
            new_shape = new cylinder_t(initial_level);
            new_shape->draw();
        }
        else if(shape_type == 3){
            new_shape = new sphere_t(initial_level);
            new_shape->draw();
        }
        else {
            new_shape = new cone_t(initial_level);
            new_shape->draw();
        }
        std::cout << shape_type <<std::endl; 
        for (size_t i = 0; i < new_shape->num_vertices; ++i) {
            v_positions[i] = glm::vec4(new_shape->vertices[i].x,new_shape->vertices[i].y,new_shape->vertices[i].z,new_shape->vertices[i].w);
        }
        for(int i = 0; i< new_shape->num_vertices; ++i){
            v_colors[i] = new_shape->colors[i];
        }
        HNode* node2 = new csX75::HNode(curr_node,new_shape->num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
        node2->change_parameters(2.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,1.0);
        node2 -> shape_pointer = new_shape;
        curr_node = node2;
    }

	void HNode::save_tree(std::ofstream& outFile) {
        outFile << "NODE_BEGIN\n";
        outFile << std::fixed << std::setprecision(6);
        outFile << "num_vertices " << num_vertices << "\n";
        outFile << "shape_type " << shape_pointer -> shape_int << "\n"; 
        outFile << tx << " " << ty << " " << tz << " "
                << rx << " " << ry << " " << rz << " "
                << sx << " " << sy << " " << sz << "\n";

        for (HNode* child : children) {
            child->save_tree(outFile);
        }
        outFile << "NODE_END\n";
	}

	void HNode::save(const std::string& filename) {
		std::ofstream outFile(filename);
		if (!outFile) {
			std::cerr << "Error opening file\n";
			return;
		}
		save_tree(outFile);
		std::cout << "MODEL SAVED to " << filename << std::endl; 
		outFile.close();
	}
	void HNode::load(const std::string& filename){
		std::ifstream in(filename);
		if (!in) {
			std::cerr << "Error opening file\n";
			return;
		}
		std::string line;
		HNode* node_pointer = NULL;
		while (std::getline(in, line)) {
            std::cout << line << "\n";
            float num_vert,tx,ty,tz,rx,ry,rz,sx,sy,sz;
            int shapet;
            std::string key;

            if(line == "NODE_BEGIN"){
                std::getline(in,line);
                std::istringstream iss(line);
                iss >> key >> num_vert;

                std::getline(in,line);
                std::istringstream iss1(line);
                iss1 >> key >> shapet;

                std::getline(in,line);
                std::istringstream iss2(line);
                iss2 >> tx >> ty >> tz >> rx >> ry >> rz >> sx >> sy >> sz;
                
                addShape(shapet);
                curr_node -> change_parameters(tx,ty,tz,rx,ry,rz,sx,sy,sz);
                node_pointer = curr_node;
            }
            if(line == "NODE_END"){
                node_pointer = node_pointer -> parent;
            }
		}
		curr_node = node_pointer -> children[0];
	}





	


}