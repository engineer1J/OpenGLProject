// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "Object.h"
#include "Camera.h"
#include "Shader.h"

#include "SOIL.h"
#include "vec.hpp"
#include "mat.hpp"
#include "transform.hpp"

using std::stof;

void init();
void display();
void reshape(int, int);
void createMenu();
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);

int removed = 0; //for additional function
bool cube = true;
bool desk = true;
bool fan = true;
bool sofa = true;
bool tv = true;
//for moving obj
bool mcube = false;
bool mdesk = false;
bool mfan = false;
bool msofa = false;
bool mtv = false;



//add variables
float MOVE_DELTA = 0.3f;
float ROTATE_DELTA = 0.1f;
//float PI = 3.141592f;
//float angle_of_fan = 0.0f;
float g_aspect = 1.0f;


GLuint program;

GLint  loc_a_vertex;
GLint  loc_a_normal;
GLint     loc_a_texcoord;

GLint  loc_u_pvm_matrix;
GLint  loc_u_view_matrix;
GLint  loc_u_model_matrix;
GLint  loc_u_normal_matrix;

GLint  loc_u_light_vector;

GLint  loc_u_light_ambient;
GLint  loc_u_light_diffuse;
GLint  loc_u_light_specular;

GLint  loc_u_material_ambient;
GLint  loc_u_material_diffuse;
GLint  loc_u_material_specular;
GLint  loc_u_material_shininess;

GLint     loc_u_texid;
GLint     loc_u_texid_score;

GLuint    texid;
GLuint    texid_score;

kmuvcl::math::mat4x4f   mat_PVM;

kmuvcl::math::vec4f light_vector      = kmuvcl::math::vec4f(10.0f, 10.0f, 10.0f);
kmuvcl::math::vec4f light_ambient     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_diffuse     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_specular    = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);

// std::string g_filename;
Object      g_model, g_desk, g_fan, g_sofa, g_tv, g_ncube;        // object
Camera		g_camera;

//value of object
float model_scale = 0.5f;
float model_angle = 0.0f;

float desk_model_scale = 1.0f;
float desk_model_angle = -90.0f;

float fan_model_scale = 1.0f;
float fan_model_angle = 0.0f;

float sofa_model_scale = 1.0f;
float sofa_model_angle = 90.0f;
 
float tv_model_scale = 1.5f;
float tv_model_angle = -90.0f;

float new_model_scale = 1.0f;
float new_model_angle = 0.0f;

//position of objects
kmuvcl::math::vec4f model_position = kmuvcl::math::vec4f(-3.3f, 2.0f, -4.0f);
kmuvcl::math::vec4f desk_model_position = kmuvcl::math::vec4f(0.0f, 0.0f, -3.0f);
kmuvcl::math::vec4f fan_model_position = kmuvcl::math::vec4f(0.0f, 3.0f, 0.0f);
kmuvcl::math::vec4f sofa_model_position = kmuvcl::math::vec4f(-3.0f, 0.0f, 0.0f);
kmuvcl::math::vec4f tv_model_position = kmuvcl::math::vec4f(2.0f, 0.0f, 0.0f);
kmuvcl::math::vec4f new_model_position = kmuvcl::math::vec4f(0.0f, 0.0f, 0.0f);

std::chrono::time_point<std::chrono::system_clock> prev, curr;

GLfloat vertices[] = {
  // front
  -1, 1, 1,   1, 1, 1,   1,-1, 1,  -1,-1, 1,
  // back
  1, 1, -1,  -1, 1, -1,  -1,-1, -1,   1,-1, -1,
  // top
  -1, 1, -1,   1, 1, -1,   1, 1, 1,  -1, 1, 1,
  // bottom
  -1,-1, 1,   1,-1, 1,   1,-1, -1,  -1,-1, -1,
  // right
  1, 1, 1,   1, 1, -1,   1,-1, -1,   1,-1, 1,
  // left
  -1, 1, -1,  -1, 1, 1,  -1,-1, 1,  -1,-1, -1,
};

GLfloat texcoords[] = {
  // front
  0,1,  1,1,  1,0,  0,0,
  // back
  0,1,  1,1,  1,0,  0,0,
  // top
  0,1,  1,1,  1,0,  0,0,
  // bottom
  0,1,  1,1,  1,0,  0,0,
  // right
  0,1,  1,1,  1,0,  0,0,
  // left
  0,1,  1,1,  1,0,  0,0,
};

GLushort indices[] = {
  //front
  0, 3, 2,   2, 1, 0,
  //back
  4, 7, 6,   6, 5, 4,
  // top
  8,11,10,  10, 9, 8,
  // bottom
  12,15,14,  14,13,12,
  //right
  16,19,18,  18,17,16,
  //left
  20,23,22,  22,21,20,
};

void saveScene(){
	FILE *fp = fopen("scene.txt", "a");
	fprintf(fp, "c %f %f %f %f %f\n", model_angle, model_scale, model_position(0), model_position(1),model_position(2));
	fprintf(fp, "d %f %f %f %f %f\n", desk_model_angle, desk_model_scale, desk_model_position(0), desk_model_position(1),desk_model_position(2));
	fprintf(fp, "f %f %f %f %f %f\n", fan_model_angle, fan_model_scale, fan_model_position(0), fan_model_position(1),fan_model_position(2));
	fprintf(fp, "s %f %f %f %f %f\n", sofa_model_angle, sofa_model_scale, sofa_model_position(0), sofa_model_position(1),sofa_model_position(2));
	fprintf(fp, "t %f %f %f %f %f\n", tv_model_angle, tv_model_scale, tv_model_position(0), tv_model_position(1),tv_model_position(2));
	fclose(fp);
}

void loadScene() { 
	FILE *fp;
	char buff[100]; 
	fp = fopen("scene.txt", "r");
	if(fp==NULL)   //정상적으로 파일을 읽었는지 검사
	{}
	else{
		while (!feof(fp)) //한줄씩 읽어내려감
	{
		fgets(buff, sizeof(buff), fp);
		char *Data;

		if(buff[0]=='c')
		{
			Data = strtok(buff, " ");
			while (Data = strtok(NULL, " "))
			{
			model_angle = stof(Data);
			model_scale = stof(Data);
			model_position(0) = stof(Data);
			model_position(1) = stof(Data);
			model_position(2) = stof(Data);
			}
		}
		if(buff[0]=='d')
		{
			Data = strtok(buff, " ");
			while (Data = strtok(NULL, " "))
			{
			desk_model_angle = stof(Data);
			desk_model_scale = stof(Data);
			desk_model_position(0) = stof(Data);
			desk_model_position(1) = stof(Data);
			desk_model_position(2) = stof(Data);
			}
		}

		if(buff[0]=='f')
		{
			Data = strtok(buff, " ");
			while (Data = strtok(NULL, " "))
			{
			fan_model_angle = stof(Data);
			fan_model_scale = stof(Data);
			fan_model_position(0) = stof(Data);
			fan_model_position(1) = stof(Data);
			fan_model_position(2) = stof(Data);
			}
		}

		if(buff[0]=='s')
		{
			Data = strtok(buff, " ");
			while (Data = strtok(NULL, " "))
			{
			sofa_model_angle = stof(Data);
			sofa_model_scale = stof(Data);
			sofa_model_position(0) = stof(Data);
			sofa_model_position(1) = stof(Data);
			sofa_model_position(2) = stof(Data);
			}
		}

		if(buff[0]=='t')
		{
			Data = strtok(buff, " ");
			while (Data = strtok(NULL, " "))
			{
			tv_model_angle = stof(Data);
			tv_model_scale = stof(Data);
			tv_model_position(0) = stof(Data);
			tv_model_position(1) = stof(Data);
			tv_model_position(2) = stof(Data);
			}
		}
	}
	}
	fclose(fp);
}

void menu1(int entryID){
	switch(entryID){
		case 1:
	if(mcube == false){
		mcube = true;
	}
	else{
		mcube = false;
	}
			break;
		case 2:
	if(mdesk == false){
		mdesk = true;
	}
	else{
		mdesk = false;
	}
			break;
		case 3:
	if(mfan == false){
		mfan = true;
	}
	else{
		mfan = false;
	}
			break;
		case 4:
	if(msofa == false){
		msofa = true;
	}
	else{
		msofa = false;
	}
			break;
		case 5:
	if(mtv == false){
		mtv = true;
	}
	else{
		mtv = false;
	}
			break;
	}
}

void menu2(int entryID){
	switch(entryID){
		case 1:
			saveScene();
			break;
		case 2:
			loadScene();
			break;
	}
}
void entireMenu(int entryID){
	switch(entryID){
		case 1:
			exit(1);
			break;
	}
}

void createMenu()
{
  GLint act_menu1 = glutCreateMenu(menu1);
  glutAddMenuEntry(" cube ", 1);
  glutAddMenuEntry(" desk ", 2);
  glutAddMenuEntry(" fan  ", 3);
  glutAddMenuEntry(" sofa ", 4);
  glutAddMenuEntry(" tv   ", 5);

  GLint act_menu2 = glutCreateMenu(menu2);
  glutAddMenuEntry(" save ", 1);
  glutAddMenuEntry(" load ", 2);

	GLint act_entireMenu = glutCreateMenu(entireMenu);
	glutAddSubMenu("move object", act_menu1);
	glutAddSubMenu("save / load", act_menu2);
	glutAddMenuEntry("exit", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char* argv[])
{
/*  if (argc > 1)
  {
    g_filename = argv[2];
  }
  else
  {
    g_filename = "./data/cube.obj";
  }    */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);  
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);  
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);  
  glutIdleFunc(idle);

  createMenu();

  if (glewInit() != GLEW_OK) 
  {
      std::cerr << "failed to initialize glew" << std::endl;
      return -1;
  }

  init();
  
  glutMainLoop();

  return 0;
}


void init()
{

  g_desk.load_simple_obj("./data/desk_with_normals.obj");
  g_fan.load_simple_obj("./data/fan_with_normals.obj");
  g_sofa.load_simple_obj("./data/sofa_with_normals.obj");
  g_tv.load_simple_obj("./data/tv_with_normals.obj");
  g_model.load_simple_obj("./data/cube.obj"); //removed filename
  g_ncube.load_simple_obj("./data/cube.obj"); //removed 
  //g_model.print();
  	
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for filled polygon rendering  

  glEnable(GL_DEPTH_TEST);

  program = Shader::create_program("./shader/phong_vert.glsl", "./shader/phong_frag.glsl");

  loc_u_pvm_matrix         = glGetUniformLocation(program, "u_pvm_matrix");
  loc_u_view_matrix        = glGetUniformLocation(program, "u_view_matrix");
  loc_u_model_matrix       = glGetUniformLocation(program, "u_model_matrix");
  loc_u_normal_matrix      = glGetUniformLocation(program, "u_normal_matrix");

  loc_u_light_vector       = glGetUniformLocation(program, "u_light_vector");
  
  loc_u_light_ambient      = glGetUniformLocation(program, "u_light_ambient");
  loc_u_light_diffuse      = glGetUniformLocation(program, "u_light_diffuse");
  loc_u_light_specular     = glGetUniformLocation(program, "u_light_specular");
  
  loc_u_material_ambient   = glGetUniformLocation(program, "u_material_ambient");
  loc_u_material_diffuse   = glGetUniformLocation(program, "u_material_diffuse");
  loc_u_material_specular  = glGetUniformLocation(program, "u_material_specular");
  loc_u_material_shininess = glGetUniformLocation(program, "u_material_shininess");

  loc_u_texid      = glGetUniformLocation(program, "u_texid");
  loc_u_texid_score= glGetUniformLocation(program, "u_texid_score");

  loc_a_vertex     = glGetAttribLocation(program, "a_vertex");
  loc_a_texcoord   = glGetAttribLocation(program, "a_texcoord");
  loc_a_normal             = glGetAttribLocation(program, "a_normal");

  curr = std::chrono::system_clock::now();
  prev = curr;
  //generate texture
  int width, height, channels;
  unsigned char* image = SOIL_load_image("desk.jpeg", 
    &width, &height, &channels, SOIL_LOAD_RGB);

  glGenTextures(1, &texid);

  glBindTexture(GL_TEXTURE_2D, texid);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

  SOIL_free_image_data(image);

  unsigned char* score = SOIL_load_image("tex.jpg", 
    &width, &height, &channels, SOIL_LOAD_RGB);

  glGenTextures(2, &texid_score);

  glBindTexture(GL_TEXTURE_2D, texid_score);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, score);
    
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

  SOIL_free_image_data(score);


}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  // Camera setting	
  kmuvcl::math::mat4x4f   mat_Proj, mat_View, mat_Model; //removed inv

  // camera extrinsic param
  mat_View = kmuvcl::math::lookAt(
		g_camera.position().x, g_camera.position().y, g_camera.position().z,				// eye position
		g_camera.center_position().x, g_camera.center_position().y, g_camera.center_position().z, // center position
		g_camera.up_direction().x, g_camera.up_direction().y, g_camera.up_direction().z			// up direction 
  );
  // camera intrinsic param
  mat_Proj = kmuvcl::math::perspective(60.0f, 1.0f, 0.001f, 10000.0f);

/*  // camera extrinsic param
  mat_View_inv = kmuvcl::math::lookAt(
    0.0f, 0.0f, 3.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);*/

  kmuvcl::math::mat3x3f mat_Normal;

if(cube == true){
//for cube
  mat_Model = kmuvcl::math::scale(model_scale, model_scale, model_scale);
  mat_Model = kmuvcl::math::rotate(model_angle*0.7f, 0.0f, 0.0f, 1.0f) * mat_Model;
  mat_Model = kmuvcl::math::rotate(model_angle,      0.0f, 1.0f, 0.0f) * mat_Model;
  mat_Model = kmuvcl::math::rotate(model_angle*0.5f, 1.0f, 0.0f, 0.0f) * mat_Model;
  mat_Model = kmuvcl::math::translate(model_position(0), model_position(1), model_position(2)) * mat_Model;

  mat_PVM = mat_Proj*mat_View*mat_Model; //removed inv
 

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

//  kmuvcl::math::mat4x4f mat_View = kmuvcl::math::inverse(mat_View_inv); //removed inv
	  
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);  
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);   
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);   
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  Shader::check_gl_error("glUniform4fv");
	
  g_model.draw(loc_a_vertex, loc_a_normal, 
    loc_u_material_ambient, loc_u_material_diffuse, 
    loc_u_material_specular, loc_u_material_shininess);
	
  //using texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texid);
  glUniform1i(loc_u_texid, 0);

  Shader::check_gl_error("draw");

  //send gpu texture coordinate
  glVertexAttribPointer(loc_a_vertex,   3, GL_FLOAT, GL_FALSE, 0, vertices);
  glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
  glVertexAttribPointer(loc_a_normal, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  
  glEnableVertexAttribArray(loc_a_vertex);
  glEnableVertexAttribArray(loc_a_texcoord);
  glEnableVertexAttribArray(loc_a_normal);

  
  glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, indices);

  glDisableVertexAttribArray(loc_a_vertex);
  glDisableVertexAttribArray(loc_a_texcoord);
  glDisableVertexAttribArray(loc_a_normal);
}

if(desk == true){
//for desk
  mat_Model = kmuvcl::math::translate(desk_model_position(0), desk_model_position(1), desk_model_position(2))*kmuvcl::math::rotate(desk_model_angle, 0.0f, 1.0f, 0.0f)*kmuvcl::math::scale(desk_model_scale, desk_model_scale, desk_model_scale);  
  
  mat_PVM = mat_Proj*mat_View*mat_Model; //removed inv

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

//  kmuvcl::math::mat4x4f mat_View = kmuvcl::math::inverse(mat_View_inv); //removed inv
	  
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);  
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);   
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);   
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  Shader::check_gl_error("glUniform4fv");
	
  g_desk.draw(loc_a_vertex, loc_a_normal, 
    loc_u_material_ambient, loc_u_material_diffuse, 
    loc_u_material_specular, loc_u_material_shininess);
	//put desk
}

/*
  //using desk texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texid);
  glUniform1i(loc_u_texid, 0);

  Shader::check_gl_error("draw");

  //send gpu texture coordinate
  glVertexAttribPointer(loc_a_vertex,   3, GL_FLOAT, GL_FALSE, 0, vertices);
  glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
  glVertexAttribPointer(loc_a_normal, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  
  glEnableVertexAttribArray(loc_a_vertex);
  glEnableVertexAttribArray(loc_a_texcoord);
  glEnableVertexAttribArray(loc_a_normal);

  
  glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, indices);

  glDisableVertexAttribArray(loc_a_vertex);
  glDisableVertexAttribArray(loc_a_texcoord);
  glDisableVertexAttribArray(loc_a_normal);
*/

if(fan == true){
  //for fan
  mat_Model = kmuvcl::math::translate(fan_model_position(0), fan_model_position(1), fan_model_position(2))*kmuvcl::math::rotate(fan_model_angle, 0.0f, 1.0f, 0.0f) *kmuvcl::math::scale(fan_model_scale, desk_model_scale, desk_model_scale);  
  
  mat_PVM = mat_Proj*mat_View*mat_Model; //removed inv

 
//  kmuvcl::math::mat3x3f mat_Normal;

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);  

  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);   
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);   
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  g_fan.draw(loc_a_vertex, loc_a_normal, 
    loc_u_material_ambient, loc_u_material_diffuse, 
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");	//put fan
}

if(sofa == true){
  //for sofa
  mat_Model = kmuvcl::math::translate(sofa_model_position(0), sofa_model_position(1), sofa_model_position(2))*kmuvcl::math::rotate(sofa_model_angle, 0.0f, 1.0f, 0.0f)*kmuvcl::math::scale(sofa_model_scale, desk_model_scale, desk_model_scale);  
  
  mat_PVM = mat_Proj*mat_View*mat_Model; //removed inv

 
//  kmuvcl::math::mat3x3f mat_Normal;
  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);  

  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);   
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);   
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  g_sofa.draw(loc_a_vertex, loc_a_normal, 
    loc_u_material_ambient, loc_u_material_diffuse, 
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");	//put sofa
}

if(tv == true){
  //for tv
  mat_Model = kmuvcl::math::translate(tv_model_position(0), tv_model_position(1), tv_model_position(2))*kmuvcl::math::rotate(tv_model_angle, 0.0f, 1.0f, 0.0f) *kmuvcl::math::scale(tv_model_scale, desk_model_scale, desk_model_scale);  
  
  mat_PVM = mat_Proj*mat_View*mat_Model; //removed inv

 
//  kmuvcl::math::mat3x3f mat_Normal;
  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);  

  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);   
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);   
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  g_tv.draw(loc_a_vertex, loc_a_normal, 
    loc_u_material_ambient, loc_u_material_diffuse, 
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");	//put tv
}

  if(removed == 5){
	//for score cube
  mat_Model = kmuvcl::math::scale(new_model_scale, new_model_scale, new_model_scale);
  mat_Model = kmuvcl::math::rotate(new_model_angle*0.7f, 0.0f, 0.0f, 1.0f) * mat_Model;
  mat_Model = kmuvcl::math::rotate(new_model_angle,      0.0f, 1.0f, 0.0f) * mat_Model;
  mat_Model = kmuvcl::math::rotate(new_model_angle*0.5f, 1.0f, 0.0f, 0.0f) * mat_Model;
  mat_Model = kmuvcl::math::translate(new_model_position(0), new_model_position(1), new_model_position(2)) * mat_Model;

  mat_PVM = mat_Proj*mat_View*mat_Model; //removed inv
 
  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

//  kmuvcl::math::mat4x4f mat_View = kmuvcl::math::inverse(mat_View_inv); //removed inv
	  
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);  
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);   
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);   
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  Shader::check_gl_error("glUniform4fv");
	
  g_ncube.draw(loc_a_vertex, loc_a_normal, 
    loc_u_material_ambient, loc_u_material_diffuse, 
    loc_u_material_specular, loc_u_material_shininess);
	
  //using texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texid);
  glUniform1i(loc_u_texid, 0);

  Shader::check_gl_error("draw");

  //send gpu texture coordinate
  glVertexAttribPointer(loc_a_vertex,   3, GL_FLOAT, GL_FALSE, 0, vertices);
  glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
  glVertexAttribPointer(loc_a_normal, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  
  glEnableVertexAttribArray(loc_a_vertex);
  glEnableVertexAttribArray(loc_a_texcoord);
  glEnableVertexAttribArray(loc_a_normal);

  
  glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, indices);

  glDisableVertexAttribArray(loc_a_vertex);
  glDisableVertexAttribArray(loc_a_texcoord);
  glDisableVertexAttribArray(loc_a_normal);
}

	glUseProgram(0);

  glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	g_aspect = width/height;

}

void keyboard(unsigned char key, int x, int y)
{
  // TODO: properly handle keyboard event
   if (key == 'a' || key == 'A')
   {
      g_camera.rotate_left(ROTATE_DELTA);
   }
   else if (key == 'd' || key == 'D')
   {
      g_camera.rotate_right(ROTATE_DELTA);
   }

//I added new function
   else if (key == 's' || key == 'S')
   {
      g_camera.digging(MOVE_DELTA);

   }
   else if (key == 'w' || key == 'W')
   {
      g_camera.flying(MOVE_DELTA);
   }

   else if (key == 'q' || key == 'Q')
   {
      g_camera.rotate_up(MOVE_DELTA);
   }
   else if (key == 'e' || key == 'E')
   {
      g_camera.rotate_down(MOVE_DELTA);
   }
   else if (key == 'z' || key == 'Z')
   {
      g_camera.remove();
   }
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	// TODO: properly handle special keyboard event
   if (key == GLUT_KEY_LEFT)
   {
      g_camera.move_left(MOVE_DELTA);
   }
   else if (key == GLUT_KEY_RIGHT)
   {
      g_camera.move_right(MOVE_DELTA);
   }
   else if (key == GLUT_KEY_UP)
   {
      g_camera.move_forward(MOVE_DELTA);
   }
   else if (key == GLUT_KEY_DOWN)
   {
      g_camera.move_backward(MOVE_DELTA);
   }
	glutPostRedisplay();
}


void idle()
{
/*  curr = std::chrono::system_clock::now();

  std::chrono::duration<float> elaped_seconds = (curr - prev);

  model_angle += 10 * elaped_seconds.count();
  
  prev = curr;*/

  model_angle += 5.0f;

  fan_model_angle += 2.0f;

  glutPostRedisplay();
}
