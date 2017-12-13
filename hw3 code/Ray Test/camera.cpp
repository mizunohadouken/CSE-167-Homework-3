#include "camera.h"


// Helper rotation function
glm::mat3 camera::rotate(const float degrees, const glm::vec3& axis)
{
	float radians = degrees*M_PI / 180.0f;
	glm::vec3 axis_normal = normalize(axis);

	glm::mat3 mRotation(0); // rotation matrix to return
	glm::mat3 mIdentity(1.0);
	glm::mat3 mAxisxAxisTranspose(axis_normal.x * axis_normal.x, axis_normal.x * axis_normal.y, axis_normal.x * axis_normal.z,
		axis_normal.x * axis_normal.y, axis_normal.y * axis_normal.y, axis_normal.y * axis_normal.z,
		axis_normal.x * axis_normal.z, axis_normal.y * axis_normal.z, axis_normal.z * axis_normal.z);
	glm::mat3 mAxisDualMatrix(0, axis_normal.z, -axis_normal.y,
		-axis_normal.z, 0, axis_normal.x,
		axis_normal.y, -axis_normal.x, 0);

	// Calculate rotation matrix
	mRotation = (mIdentity * cos(radians))
		+ ((1 - cos(radians)) * mAxisxAxisTranspose)
		+ (sin(radians) * mAxisDualMatrix);

	// You will change this return call
	return mRotation;
}

// TODO verify?
glm::mat4 camera::make_coordinate_frame(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
	// Create coordinate frame for camera
	glm::vec3 w = normalize(eye-center);
	glm::vec3 u = normalize(cross(up, w));
	glm::vec3 v = cross(w, u);

	glm::mat4 coordinate_frame_mat(u.x, v.x, w.x, 0,
		u.y, v.y, w.y, 0,
		u.z, v.z, w.z, 0,
		0, 0, 0, 1);

	return coordinate_frame_mat;
}

glm::mat4 camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	glm::mat4 ret;

	// New, to implement the perspective transform as well.  
	float d = 1.0f / (tan((fovy * M_PI) / (2 * 180)));
	float A = -(zFar + zNear) / (zFar - zNear);
	float B = -(2 * zFar * zNear) / (zFar - zNear);

	ret[0][0] = d / aspect;
	ret[1][1] = d;
	ret[2][2] = A;
	ret[3][2] = B;
	ret[2][3] = -1;
	ret[3][3] = 0;

	return ret;
}

glm::mat4 camera::scale(const float &sx, const float &sy, const float &sz)
{
	glm::mat4 ret = glm::mat4(1);

	// Implement scaling 
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = sz;

	return ret;
}

glm::mat4 camera::translate(const float &tx, const float &ty, const float &tz)
{
	glm::mat4 ret = glm::mat4(1);

	// Implement translation 
	ret[3][0] = tx;
	ret[3][1] = ty;
	ret[3][2] = tz;

	return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

glm::vec3 camera::upvector(const glm::vec3 &up, const glm::vec3 & zvec)
{
	glm::vec3 x = glm::cross(up, zvec);
	glm::vec3 y = glm::cross(zvec, x);
	glm::vec3 ret = glm::normalize(y);
	return ret;
}

// TODO verify ray hitting center of pixel
ray camera::create_ray(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float fovy, int width, int height, float i_pixel, float j_pixel)
{
	glm::mat4 coor_frame = camera::make_coordinate_frame(eye, center, up);

	// define vectors u, v, w
	glm::vec3 u = glm::vec3(coor_frame[0][0], coor_frame[1][0], coor_frame[2][0]);
	glm::vec3 v = glm::vec3(coor_frame[0][1], coor_frame[1][1], coor_frame[2][1]);
	glm::vec3 w = glm::vec3(coor_frame[0][2], coor_frame[1][2], coor_frame[2][2]);

	fovy = fovy*M_PI / 180.0f;  // Convert degrees to rads

	float tan_fovx2 = tan(fovy / 2.0f)*width / height;
//	float alpha = tan_fovx2 * ((j_pixel +.5f - (width / 2.0f)) / (width / 2.0f)); // TODO check pixel offset?
//	float beta = tan(fovy / 2.0f) * (((height / 2.0f) - i_pixel - .5f) / (height / 2.0f)); // TODO check pixel offset?

	float alpha = tan_fovx2 * ((j_pixel - (width / 2.0f)) / (width / 2.0f)); // TODO check pixel offset?
	float beta = tan(fovy / 2.0f) * (((height / 2.0f) - i_pixel) / (height / 2.0f)); // TODO check pixel offset?

	glm::vec3 ray_dir_temp = alpha*u + beta*v - w;
	ray ray_shot(eye, glm::normalize(ray_dir_temp));

	return ray_shot;
}




camera::camera()
{

}

camera::~camera()
{

}

glm::mat4 camera::lookAt(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & up)
{
	// Create coordinate frame for camera
	//	vec3 w = normalize(eye - center);
	glm::vec3 w = normalize(eye);
	glm::vec3 u = normalize(cross(up, w));
	glm::vec3 v = cross(w, u);

	// Define Rotation Matrix
	glm::mat4 mCameraRotation(u.x, v.x, w.x, 0,
		u.y, v.y, w.y, 0,
		u.z, v.z, w.z, 0,
		0, 0, 0, 1);

	// Define translation Matrix
	glm::mat4 mCameraTranslation(1);
	mCameraTranslation[3][0] = -eye.x;
	mCameraTranslation[3][1] = -eye.y;
	mCameraTranslation[3][2] = -eye.z;

	// Apply translation for camera (eye) location
	glm::mat4 mM = mCameraRotation * mCameraTranslation;

	return mM;
}

ray::ray()
{
}

ray::ray(glm::vec3 ray_origin_con, glm::vec3 ray_dir_con)
{
	ray_origin = ray_origin_con;
	ray_dir = ray_dir_con;
}
