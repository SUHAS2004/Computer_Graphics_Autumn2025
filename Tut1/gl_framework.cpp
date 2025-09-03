#include "gl_framework.hpp"


extern GLfloat c_xrot,c_yrot,c_zrot,c_xpos,c_ypos,c_zpos;
extern bool enable_perspective;
extern csX75::HNode* node1, *node2, *node3,*curr_node,*root_node,*tail_node;
extern float delta_factor;
extern bool solid,enable_perspective;
extern int new_shape;
namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_M  && action == GLFW_PRESS)
      CurrentView = MODELLING;        
    else if (key == GLFW_KEY_I  && action == GLFW_PRESS)
      CurrentView = INSPECTION;

    else if (key == GLFW_KEY_T  && action == GLFW_PRESS)
      CurrentMode = TRANSLATE;
    else if (key == GLFW_KEY_G  && action == GLFW_PRESS)
      CurrentMode = SCALE;
    else if (key == GLFW_KEY_R  && action == GLFW_PRESS)
      CurrentMode = ROTATE;
    else if (key == GLFW_KEY_X  && action == GLFW_PRESS)
      CurrentAxis = X_AXIS;
    else if (key == GLFW_KEY_Y  && action == GLFW_PRESS)
      CurrentAxis = Y_AXIS;
    else if (key == GLFW_KEY_Z  && action == GLFW_PRESS)
      CurrentAxis = Z_AXIS;
    if (key == GLFW_KEY_W  && action == GLFW_PRESS)
      solid = !solid;
    if (key == GLFW_KEY_P  && action == GLFW_PRESS)
      enable_perspective = !enable_perspective;
    else if(CurrentView == MODELLING){
      if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        new_shape = 1;
      else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        new_shape = 2;
      else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        new_shape = 3;
      else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        new_shape = 4;
      else if (key == GLFW_KEY_5 && action == GLFW_PRESS){
        curr_node->delete_node();
        csX75::HNode* dummy;
        dummy = curr_node -> parent;
        delete curr_node;
        curr_node = dummy;  
      }
      else if (key == GLFW_KEY_S  && action == GLFW_PRESS){
        std::string filename;
        std::cout << "ENTER FILE NAME TO BE SAVED : ";
        std::cin >> filename;
        std::cout << std::endl;
        root_node -> save(filename);    
      }
      else if (key == GLFW_KEY_L  && action == GLFW_PRESS){
        std::string filename;
        std::cout << "ENTER FILE NAME TO BE LOADED : ";
        std::cin >> filename;
        std::cout << std::endl;
        curr_node -> load(filename);
        //curr_node = curr_node -> children[0];
      }
      else if (key == GLFW_KEY_E  && action == GLFW_PRESS){
        curr_node = tail_node;
      }
      else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        curr_node->dec();
      } 
      else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        curr_node->inc();
      }   
    }
    else{
      if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        if(CurrentAxis == X_AXIS && CurrentMode == ROTATE)
          c_xrot = c_xrot - delta_factor;
        else if(CurrentAxis == Y_AXIS && CurrentMode == ROTATE)
          c_yrot = c_yrot - delta_factor;
        else if(CurrentAxis == Z_AXIS && CurrentMode == ROTATE)
          c_zrot = c_zrot - delta_factor;
        if(CurrentAxis == X_AXIS && CurrentMode == TRANSLATE)
          c_xpos = c_xpos - delta_factor;
        else if(CurrentAxis == Y_AXIS && CurrentMode == TRANSLATE)
          c_ypos = c_ypos - delta_factor;
        else if(CurrentAxis == Z_AXIS && CurrentMode == TRANSLATE)
          c_zpos = c_zpos - delta_factor;
      }
      else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        if(CurrentAxis == X_AXIS && CurrentMode == ROTATE)
          c_xrot = c_xrot + delta_factor;
        else if(CurrentAxis == Y_AXIS && CurrentMode == ROTATE)
          c_yrot = c_yrot + delta_factor;
        else if(CurrentAxis == Z_AXIS && CurrentMode == ROTATE)
          c_zrot = c_zrot + delta_factor;
        if(CurrentAxis == X_AXIS && CurrentMode == TRANSLATE)
          c_xpos = c_xpos + delta_factor;
        else if(CurrentAxis == Y_AXIS && CurrentMode == TRANSLATE)
          c_ypos = c_ypos + delta_factor;
        else if(CurrentAxis == Z_AXIS && CurrentMode == TRANSLATE)
          c_zpos = c_zpos + delta_factor;
      }
    }
    if (key == GLFW_KEY_EQUAL  && action == GLFW_PRESS)
      delta_factor = delta_factor*2.0;
    if (key == GLFW_KEY_MINUS  && action == GLFW_PRESS)
      delta_factor = delta_factor/2.0;
    // else if (key == GLFW_KEY_C  && action == GLFW_PRESS)
    //   c_zrot += 1.0;

  }
};  
  


