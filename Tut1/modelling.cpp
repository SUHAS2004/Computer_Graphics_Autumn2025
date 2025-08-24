#include "modelling.hpp"
#include "cmath"
#include <algorithm>


GLuint shaderProgram;
GLuint uModelViewMatrix;

const int num_vertices = 200;
glm::vec4 v_positions[num_vertices];
glm::vec4 v_colors[num_vertices];


glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;
glm::mat4 rotation_matrix;


glm::mat4 model_matrix;
glm::mat4 view_matrix;



constexpr double PI = 3.14159265358979323846;


class shape_t {
    public:
        std::vector <glm::vec4> vertices;
        std::vector <glm::vec4> colors;
        int num_vertices;
        enum shape_type {SPHERE_SHAPE, CYLINDER_SHAPE, BOX_SHAPE, CONE_SHAPE};
        shape_type shape;
        unsigned int level;
        
        virtual void draw() {};
        shape_t(unsigned int level) // normal constructor that can only be defined from daughter class
        {      
            if (level < 0 or level > 4) {
                throw std::invalid_argument("tessellation level must be between 0 and 4");
            }
            this -> level = level;
        }
};

class sphere_t : public shape_t {
    public:
        sphere_t(unsigned int level) : shape_t(level) {
            shape = SPHERE_SHAPE;
        }
        void draw() override {
            int num_vert = 1 + 4*(pow(2,level+1)-1); // by sum of geometric series
            glm::vec4 vertex[num_vert];
            glm::vec4 *current = vertex;
            for(int i = 0; i < level + 1; i++){ // iterate for each level
                int points_per_plane = pow(2,(2 + level - i)); // number of points in the circle(plane)
                float z_val = i*1/(level+1); // z value of vertex
                float radius = sqrt(1-pow(2,z_val)); // radius of the circle
                for(int j = 0; j < points_per_plane; j++){ // iterate for each equidistant point on circle
                     current -> x = sin(j*PI/points_per_plane);
                     current -> y = cos(j*PI/points_per_plane);
                     current -> z = z_val;
                     current++;
                }
            }
        }


};

class cone_t : public shape_t {
    public:
        cone_t(unsigned int level) : shape_t(level) {
            shape = CONE_SHAPE;
        }
        void draw() override {
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

};

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("07_vshader.glsl");
  std::string fragment_shader_file("07_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

  //note that the buffers are initialized in the respective constructors
  cone_t cone1(3);
  cone1.draw();
  for (size_t i = 0; i < cone1.vertices.size(); ++i) {
    v_positions[i] = glm::vec4(cone1.vertices[i].x,cone1.vertices[i].y,cone1.vertices[i].z,cone1.vertices[i].w);
    }
  for(int i = 0; i<cone1.colors.size(); ++i){
    v_colors[i] = cone1.colors[i];
    }
  node1 = new csX75::HNode(NULL,num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  root_node = node1;
  curr_node = node1;

}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  //Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  if(enable_perspective)
    projection_matrix = glm::frustum(-7.0, 7.0, -7.0, 7.0, 1.0, 7.0);
    //projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  else
    projection_matrix = glm::ortho(-7.0, 7.0, -7.0, 7.0, -5.0, 5.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);
  node1->render_tree();

}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(512, 512, "CS475/CS675 Tutorial 7: Hierarchical Modelling", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;
  
  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);
  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  
  //Initialize GL state
  csX75::initGL();
  initBuffersGL();


  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}
