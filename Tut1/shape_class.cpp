#include "cmath"
#include <algorithm>
#include "shape_class.hpp"

constexpr double PI = 3.14159265358979323846;
shape_t::shape_t(unsigned int level) // normal constructor that can only be defined from daughter class
{      
    if (level < 0 or level > 7) {
        throw std::invalid_argument("tessellation level must be between 0 and 4");
    }
    this -> level = level;
}

shape_t::~shape_t() = default;  // out-of-line definition anchors vtable/RTTI

sphere_t::sphere_t (unsigned int level) : shape_t(level) {
    shape = SPHERE_SHAPE;
    shape_int = 3;
}
void sphere_t::draw(){
    int num_vert = (1 + 4*(pow(2,level+1)-1) + level); // by sum of geometric series
    int num_traingles[8] = {12,60,132,276,564,1140,2292,4596}; //pre computed number of overlapping vertices required
    num_vertices = num_traingles[level]- 5;
    //std::cout << "num of overlapping vertices : "<< num_vertices <<std::endl;
    glm::vec4 vertex[2*num_vert];
    int last_pos = 0; // position of the last added element
    for(int i = 0; i < level + 1; i++){ // iterate for each level
        int points_per_plane = pow(2,(2 + level - i)); // number of points in the circle(plane)
        std::cout << "points in plane "<< i <<" are : " << points_per_plane <<std::endl;
        float z_val = 1.0f - 1.0f/pow(2,i); // z value of vertex
        std::cout << "z value : "<< i/(level+1.0f) <<"  "<<i<< std::endl;
        float radius = sqrt(1.0f-pow(z_val,2)); // radius of the circle   
        for(int j = 0; j < points_per_plane + 1; j++){ // iterate for each equidistant point on circle
            vertex[last_pos].x = radius*sin(2*j*PI/points_per_plane);
            vertex[last_pos].y = radius*cos(2*j*PI/points_per_plane);
            vertex[last_pos].z = z_val;
            vertex[last_pos].w = 1;
            //std::cout << vertex[last_pos].x <<"-----"<< vertex[last_pos].y <<"------"<< vertex[last_pos].z <<std::endl;
            last_pos++;
        }
    }
    //std::cout << "last pos : "<< last_pos << std::endl;
    last_pos = 0;
    int last_pos_slow = 0;
    for(int i = 0; i < level; i++){ // iterate for each level
        int points_per_plane = pow(2,(2 + level - i)); // number of points in the circle(plane)
        for(int j = 0; j < points_per_plane/2; j++){ // iterate for each equidistant point on circle
            vertices.push_back(vertex[last_pos ]);
            vertices.push_back(vertex[last_pos+ 1 ]);
            vertices.push_back(vertex[points_per_plane + 1 + last_pos_slow]);
            colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
            colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
            colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));

            vertices.push_back(vertex[last_pos + 1 ]);
            vertices.push_back(vertex[last_pos + 2 ]);
            vertices.push_back(vertex[points_per_plane + 2 + last_pos_slow]);
            colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
            colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
            colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
            vertices.push_back(vertex[points_per_plane + 1 + last_pos_slow]);
            vertices.push_back(vertex[points_per_plane + 2 + last_pos_slow]);
            std::cout << "last pos2 : "<< last_pos_slow + 2 + points_per_plane  << std::endl;
            vertices.push_back(vertex[last_pos + 1 ]);
            colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
            colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
            colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
            last_pos = last_pos + 2;
            last_pos_slow++;
            
        }
        last_pos++;
        last_pos_slow = last_pos;
    }
    //last_pos = num_vert - 6;
    //std::cout << "last pos1 : "<< last_pos << std::endl;
    glm::vec4 top;
    top.x = 0;
    top.y = 0;
    top.z = 1;
    top.w = 1;
    for(int i = 0; i < 4; i++){
        vertices.push_back(vertex[last_pos]);
        //std::cout << vertex[last_pos].x <<"-----"<< vertex[last_pos].y <<"------"<< vertex[last_pos].z <<std::endl;
        vertices.push_back(vertex[last_pos+1]);
        vertices.push_back(top);
        colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
        colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
        colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
        last_pos++;
    }
    unsigned int size = vertices.size();
    for(int i = 0; i < size; i++){
        glm::vec4 dummy = vertices[i];
        dummy.z = -dummy.z;
        vertices.push_back(dummy);
        colors.push_back(colors[i]);
    }
    num_vertices = 2*num_vertices;
}


cylinder_t::cylinder_t(unsigned int level) : shape_t(level){
    shape = CYLINDER_SHAPE;
    shape_int = 2;
}
void cylinder_t::draw() {
    int points_on_circle = pow(2,(level + 2));
    glm::vec4 vertex[2*points_on_circle];
    num_vertices = points_on_circle*12;
    // add each point on the unit circle
    for(int i = 0; i < points_on_circle + 1; i++){
        vertex[i].x = sin(2*i*PI/(points_on_circle));
        vertex[i].y = cos(2*i*PI/(points_on_circle));
        vertex[i].z = 0;
        vertex[i].w = 1;
    }
    //define the center of the cone
    glm::vec4 center;
    center.x = 0;
    center.y = 0;
    center.z = 0;
    center.w = 1;

    // generate curved surface from points on circle and vertex
    for(int i = 0; i < points_on_circle; i++){
        vertices.push_back(vertex[i]);
        vertices.push_back(vertex[i + 1]);
        vertex[i].z = 1;
        vertex[i+1].z = 1;
        vertices.push_back(vertex[i]);
        vertices.push_back(vertex[i]);
        vertices.push_back(vertex[i+1]);
        vertex[i].z = 0;
        vertex[i+1].z = 0;
        vertices.push_back(vertex[i+1]);

    }

    for(int i = 0; i < 2*points_on_circle; i++){
        colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
        colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
        colors.push_back(glm::vec4(0.5f,0.2f,0.5f,1.0f));

    }
    //generate flat bottom form points on circle and circle centre
    for(int i = 0; i < points_on_circle; i++){
        vertices.push_back(vertex[i]);
        vertices.push_back(vertex[i + 1]);
        vertices.push_back(center);
        center.z = 1;
        vertex[i].z = 1;
        vertex[i+1].z = 1;
        vertices.push_back(vertex[i]);
        vertices.push_back(vertex[i + 1]);
        vertices.push_back(center);
        vertex[i].z = 0;
        vertex[i+1].z = 0;
        center.z = 0;
    }


    for(int i = 0; i < 2*points_on_circle; i++){
        colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
        colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
        colors.push_back(glm::vec4(0.5f,0.5f,0.5f,1.0f));
    }

}



cone_t::cone_t(unsigned int level) : shape_t(level) {
    shape = CONE_SHAPE;
    shape_int = 1;
}
void cone_t::draw() {
    int points_on_circle = pow(2,(level + 2));
    glm::vec4 vertex[points_on_circle];
    num_vertices = points_on_circle*6;
    // add each point on the unit circle
    for(int i = 0; i < points_on_circle; i++){
        vertex[i].x = sin(2*i*PI/(points_on_circle));
        vertex[i].y = cos(2*i*PI/(points_on_circle));
        vertex[i].z = 0;
        vertex[i].w = 1;
    }

    //define the vertex of the cone
    glm::vec4 cone_vertex;
    cone_vertex.x = 0;
    cone_vertex.y = 0;
    cone_vertex.z = 1;
    cone_vertex.w = 1;

    //define the center of the cone
    glm::vec4 cone_center;
    cone_center.x = 0;
    cone_center.y = 0;
    cone_center.z = 0;
    cone_center.w = 1;

    // generate curved surface from points on circle and vertex
    for(int i = 0; i < points_on_circle-1; i++){
        vertices.push_back(vertex[i]);
        vertices.push_back(vertex[i + 1]);
        vertices.push_back(cone_vertex);
    }
    vertices.push_back(vertex[points_on_circle-1]);
    vertices.push_back(vertex[0]);
    vertices.push_back(cone_vertex);

    for(int i = 0; i < points_on_circle; i++){
        colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
        colors.push_back(glm::vec4(0.0f,0.2f,0.5f,1.0f));
        colors.push_back(glm::vec4(0.5f,0.2f,0.5f,1.0f));

    }

    //generate flat bottom form points on circle and circle centre
    for(int i = 0; i < points_on_circle-1; i++){
        vertices.push_back(vertex[i]);
        vertices.push_back(vertex[i + 1]);
        vertices.push_back(cone_center);

    }
    vertices.push_back(vertex[points_on_circle-1]);
    vertices.push_back(vertex[0]);
    vertices.push_back(cone_center);

    for(int i = 0; i < points_on_circle; i++){
        colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
        colors.push_back(glm::vec4(0.5f,0.5f,0.0f,1.0f));
        colors.push_back(glm::vec4(0.5f,0.5f,0.5f,1.0f));
    }

}

box_t::box_t(unsigned int level) : shape_t(level) {
    shape = BOX_SHAPE;
    shape_int = 4;
}
void box_t::draw() {



    num_vertices = 36*(level + 1)*(level + 1)*(level +1);

    glm::vec4 v_positions[8] = {
    glm::vec4(0.0, -0.1, 0.1, 1.0),
    glm::vec4(0.0, 0.1, 0.1, 1.0),
    glm::vec4(0.2, 0.1, 0.1, 1.0),
    glm::vec4(0.2, -0.1, 0.1, 1.0),
    glm::vec4(0.0, -0.1, -0.1, 1.0),
    glm::vec4(0.0, 0.1, -0.1, 1.0),
    glm::vec4(0.2, 0.1, -0.1, 1.0),
    glm::vec4(0.2, -0.1, -0.1, 1.0)};

    //RGBA colors
    glm::vec4 init_colors[8] = {
    glm::vec4(0.5f,0.5f,0.0f,1.0f),
    glm::vec4(0.0f,0.2f,0.5f,1.0f),
    glm::vec4(0.5f,0.5f,0.0f,1.0f),
    glm::vec4(0.0f,0.2f,0.5f,1.0f),
    glm::vec4(0.5f,0.5f,0.0f,1.0f),
    glm::vec4(0.0f,0.2f,0.5f,1.0f),
    glm::vec4(0.5f,0.5f,0.0f,1.0f),
    glm::vec4(0.0f,0.2f,0.5f,1.0f)
    };

    glm::vec4 init_positions[8];
    // generate 12 triangles: 36 vertices and 36 colors
    for(float i = 0; i< level+1; i++){
        for(float j = 0; j<level+1; j++){
            for(float k = 0; k<level+1; k++){
                for(int n = 0; n<8; n++){
                    init_positions[n] = glm::translate(glm::mat4(1.0f),glm::vec3(i/5,j/5,k/5))*v_positions[n];
                }
                quad( 1, 0, 3, 2, init_positions, init_colors );
                quad( 2, 3, 7, 6, init_positions, init_colors );
                quad( 3, 0, 4, 7, init_positions, init_colors );
                quad( 6, 5, 1, 2, init_positions, init_colors );
                quad( 4, 5, 6, 7, init_positions, init_colors );
                quad( 5, 4, 0, 1, init_positions, init_colors );
            }
        }
    }

}
// quad generates two triangles for each face and assigns colors to the vertices
void box_t::quad(int a, int b, int c, int d, glm::vec4* init_positions, glm::vec4* init_colors)
{    


    int tri_idx=0;
    colors.push_back(init_colors[a]); vertices.push_back(init_positions[a]); tri_idx++;
    colors.push_back(init_colors[b]); vertices.push_back(init_positions[b]); tri_idx++;
    colors.push_back(init_colors[c]); vertices.push_back(init_positions[c]); tri_idx++;
    colors.push_back(init_colors[a]); vertices.push_back(init_positions[a]); tri_idx++;
    colors.push_back(init_colors[c]); vertices.push_back(init_positions[c]); tri_idx++;
    colors.push_back(init_colors[d]); vertices.push_back(init_positions[d]); tri_idx++;
}

