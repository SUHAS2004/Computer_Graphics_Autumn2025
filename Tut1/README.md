3D Modelling/Rendering Interface
Author: Suhas G(22b1826)

Intro:
This project demonstrates rendering of basic 3D shapes using the
OpenGL graphics library. The rendered shapes can be
transformed(translated, rotated and scaled). This project
demonstrates how the modern 3D graphics world is made up of
triangular primitives.

Hierarchy Tree Model:
All the shapes added in the application are added as a daughter
of the previous shape. Which makes the previously added shape
the parent of the newly added shape. Any transformation applied
to the parent is also applied to the daughter node.

How to Run:
	1.​Make sure that OpenGL and gcc are installed properly in
	your pc
	2.​Pull the Tut1 directory from github to your local machine
	3.​Run the make script by typing “make” in your linux(preferably
	ubuntu terminal)
	4.​Now you should be able to see a “modelling” executable in
	the directory.
	5.​Run the executable by entering “./modelling” in your terminal
	6.​A new window will appear where all your models go.
	
Keyboard shortcuts:
	1.​‘M’,’I’ - Modelling and Inspection mode
#In Modelling mode:
	2.​‘1’,’2’,’3’,’4’ - to add cone, cylinder, sphere and box respectively
	3.​ ‘5’ - remove the last added shape
#Transformations apply to the shape in Modelling mode and to the camera in Inspection mode
	4.​‘R’,’T’,’G’ - Rotation, Translation and Scaling mode, applies to
	the last added shape
	5.​‘X’,’Y’,’Z’ - Axis along which the transformation should be applied
	6.​‘Left_arrow’,’right_arrow’ - increment and decrement the
	transformation in the respective direction
	7.​‘+’,’-’ - increase and decrease the precision of the increment and
	decrement operation by a factor of 2.
	8.​‘S’ - Save model to a local file. Name to be given in the terminal.
	9.​‘L’ - Load a saved model from the file. Name of the file to be
	given in the terminal.
	10.​ ‘E’ - Edit the loaded model after loading it. Initially the root
	node is the current which make the transformations apply to the
	whole model. Pressing E allows you to edit the last added shape
	to the model.
	11.​ ‘P’ - toggle projection view
	
#Note: The Tessellation level and solid/wireframe can be modified by
#the initial_level and solid variables respectively to any value between
#0 and 7 in the modelling.hpp file before make.

Acknowledging supporters:
1.​Rahul Lande(22b1826)
2.​Nikil S(22b0963)
3.​Perplexity (used only for cpp syntax)

Example Models:
1.Pawn.mod:![Alt text](Pawn.png)

2.King.mod:![Alt text](King.png)


Declaration:
I solemnly swear that I am upto no good :) yet I have not copied any
code from other people and have adhered to the course’s policy and
conduct.
-Suhas
