#include "Camera.h"
#include <glm/ext.hpp>
#include "math.h"
#include "transform.hpp"
#include "vec.hpp"

#include <iostream>

float DIRECTION = -1.0f;
float PI = 3.141592f;
bool collision = false;
bool removing = false;

extern kmuvcl::math::vec4f model_position;
extern kmuvcl::math::vec4f desk_model_position;
extern kmuvcl::math::vec4f fan_model_position;
extern kmuvcl::math::vec4f sofa_model_position;
extern kmuvcl::math::vec4f tv_model_position;
extern int removed;
extern bool cube;
extern bool desk;
extern bool fan;
extern bool sofa;
extern bool tv;
extern bool mcube;
extern bool mdesk;
extern bool mfan;
extern bool msofa;
extern bool mtv;


const glm::vec3 Camera::center_position() const
{
  return  glm::vec3(position_.x + front_dir_.x,
                 position_.y + front_dir_.y, 
                 position_.z + front_dir_.z);
}


void Camera::remove(){
	if(removing == false){
	removing = true;
	}
	else if(removing == true){
	removing = false;	
	}
}

void Camera::Check_collision(float x_position, float z_position, float y_position){
	if(((model_position(0)-1.0f)<= x_position) && (x_position<=(model_position(0)+1.0f)) && ((model_position(2)-1.0f)<= z_position) && (z_position <=(model_position(2)+1.0f)) && ((model_position(1)-2.0f)<= y_position) && (y_position<=(model_position(1)+1.0f))){
	collision = true;
	}
	else if(((desk_model_position(0)-1.5f)<= x_position) && (x_position<=(desk_model_position(0)+1.5f)) && ((desk_model_position(2)-1.2f)<= z_position) && (z_position <=(desk_model_position(2)+2.0f)) && ((desk_model_position(1)-0.1f)<= y_position) && (y_position<=(desk_model_position(1)+2.0f))){
	collision = true;
	}
	else if(((fan_model_position(0)-1.0f)<= x_position) && (x_position<=(fan_model_position(0)+1.0f)) && ((fan_model_position(2)-1.0f)<= z_position) && (z_position <=(fan_model_position(2)+1.0f)) && ((fan_model_position(1)-0.3f)<= y_position) && (y_position<=(fan_model_position(1)+0.2f))){
	collision = true;
	}
	else if(((sofa_model_position(0)-0.7f)<= x_position) && (x_position<=(sofa_model_position(0)+0.7f)) && ((sofa_model_position(2)-2.3f)<= z_position) && (z_position <=(sofa_model_position(2)+2.3f)) && ((sofa_model_position(1)-0.1f)<= y_position) && (y_position<=(sofa_model_position(1)+1.2f))){
	collision = true;
	}
	else if(((tv_model_position(0)-0.3f)<= x_position) && (x_position<=(tv_model_position(0)+0.3f)) && ((tv_model_position(2)-2.5f)<= z_position) && (z_position <=(tv_model_position(2)+2.5f)) && ((tv_model_position(1)-0.1f)<= y_position) && (y_position<=(tv_model_position(1)+1.7f))){
	collision = true;
	}
}

void Camera::remove(float x_position, float z_position, float y_position){
	if(((model_position(0)-1.0f)<= x_position) && (x_position<=(model_position(0)+1.0f)) && ((model_position(2)-1.0f)<= z_position) && (z_position <=(model_position(2)+1.0f)) && ((model_position(1)-2.0f)<= y_position) && (y_position<=(model_position(1)+1.0f))){
	cube = false;
	removed += 1;
	}
	else if(((desk_model_position(0)-1.5f)<= x_position) && (x_position<=(desk_model_position(0)+1.5f)) && ((desk_model_position(2)-1.2f)<= z_position) && (z_position <=(desk_model_position(2)+2.0f)) && ((desk_model_position(1)-0.1f)<= y_position) && (y_position<=(desk_model_position(1)+2.0f))){
	desk = false;
	removed += 1;
	}
	else if(((fan_model_position(0)-1.0f)<= x_position) && (x_position<=(fan_model_position(0)+1.0f)) && ((fan_model_position(2)-1.0f)<= z_position) && (z_position <=(fan_model_position(2)+1.0f)) && ((fan_model_position(1)-0.3f)<= y_position) && (y_position<=(fan_model_position(1)+0.2f))){
	fan = false;
	removed += 1;
	}
	else if(((sofa_model_position(0)-0.7f)<= x_position) && (x_position<=(sofa_model_position(0)+0.7f)) && ((sofa_model_position(2)-2.3f)<= z_position) && (z_position <=(sofa_model_position(2)+2.3f)) && ((sofa_model_position(1)-0.1f)<= y_position) && (y_position<=(sofa_model_position(1)+1.2f))){
	sofa = false;
	removed += 1;
	}
	else if(((tv_model_position(0)-0.3f)<= x_position) && (x_position<=(tv_model_position(0)+0.3f)) && ((tv_model_position(2)-2.5f)<= z_position) && (z_position <=(tv_model_position(2)+2.5f)) && ((tv_model_position(1)-0.1f)<= y_position) && (y_position<=(tv_model_position(1)+1.7f))){
	tv = false;
	removed += 1;
	}
}

void Camera::move_forward(float delta)
{
	Check_collision(position_.x + delta * front_dir_.x, position_.z + delta * front_dir_.z, position_.y);
	if (collision == false || mcube==true||mdesk==true || mfan==true || msofa ==true || mtv == true){
	position_.x += delta * front_dir_.x;	
	position_.z += delta * front_dir_.z;
	}
	if(removing == true){
	position_.x += delta * front_dir_.x;	
	position_.z += delta * front_dir_.z;
	remove(position_.x, position_.z, position_.y);
	}
	collision = false;
	//for moving obj
	move_obj();

}

void Camera::move_backward(float delta)
{
  move_forward(-delta);
}

void Camera::move_left(float delta)
{
	Check_collision(position_.x - delta * right_dir_.x, position_.z - delta * right_dir_.z, position_.y);
	if (collision == false || mcube==true||mdesk==true || mfan==true || msofa ==true || mtv == true){
	position_.x -= delta * right_dir_.x;
	position_.z -= delta * right_dir_.z;
	}
	if(removing == true){
	position_.x -= delta * right_dir_.x;	
	position_.z -= delta * right_dir_.z;
	remove(position_.x, position_.z, position_.y);
	}
	collision = false;
	//for moving obj
	move_obj();
}

void Camera::move_right(float delta)
{
  move_left(-delta);
}

void Camera::rotate_left(float delta)
{

	front_dir_ = glm::rotateY(front_dir_, -delta);
	right_dir_ = glm::cross(front_dir_, up_dir_);

}

void Camera::rotate_right(float delta)
{
  rotate_left(-delta);
}

//new function
void Camera::rotate_up(float delta)
{
	if ((pitch_ + delta) >= PI/2){
		pitch_ = PI/2;
	}	//use PI for calculate radian
		//prevent overrotating
	else {
	pitch_ += delta;
	}
	front_dir_.y = (DIRECTION)*-sin(pitch_);
}

void Camera::rotate_down(float delta)
{
	if ((pitch_ - delta) <= -PI/2){
		pitch_ = -PI/2;
	}	//use PI for calculate radian
		//prevent overrotating
	else {
	pitch_ -= delta;
	}
	front_dir_.y = (DIRECTION)*-sin(pitch_);
}

void Camera::digging(float delta)
{	
	Check_collision(position_.x, position_.z, position_.y - delta * up_direction().y);
	if (collision == false || mcube==true||mdesk==true || mfan==true || msofa ==true || mtv == true){
	position_.y -= delta * up_direction().y;
	}
	if(removing == true){
	position_.y -= delta * up_direction().y;
	remove(position_.x, position_.z, position_.y);
	}
	collision = false;
	//for moving obj
	move_obj();

}

void Camera::flying(float delta)
{
  digging(-delta);
}

void Camera::move_obj(){
if(mcube == true){
	model_position(0) = position_.x;
	model_position(1) = position_.y - 1.0f;
	model_position(2) = position_.z;
}
else if(mdesk == true){
	desk_model_position(0) = position_.x;
	desk_model_position(1) = position_.y - 1.0f;
	desk_model_position(2) = position_.z;
}

else if(mfan == true){
	fan_model_position(0) = position_.x;
	fan_model_position(1) = position_.y - 1.0f;
	fan_model_position(2) = position_.z;
}

else if(msofa == true){
	sofa_model_position(0) = position_.x;
	sofa_model_position(1) = position_.y - 1.0f;
	sofa_model_position(2) = position_.z;
}

else if(mtv == true){
	tv_model_position(0) = position_.x;
	tv_model_position(1) = position_.y - 1.0f;
	tv_model_position(2) = position_.z;
}

}
