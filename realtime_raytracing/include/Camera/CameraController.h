#pragma once
#include "MovCamera.h"

class CameraController
{
	MovCamera& cam;
	float cam_speed = 1.0f;
	float sensivity = 0.5f;
	float pitch = 0.0f;
	float yaw = -90.0f;
public:

	CameraController() = default;
	CameraController(MovCamera& _cam): cam(_cam) {}

	void set_cam_direction(float _pitch, float _yaw) {
		pitch = _pitch;
		yaw = _yaw;
	}

	void rotate_cam(float offset_x, float offset_y, float dt) {
		float velocity = sensivity;
		pitch -= offset_y * velocity;
		yaw += offset_x * velocity;
		
		if (pitch > 89.0f) { pitch = 89.0f; }
		if (pitch < -89.0f) { pitch = -89.0f; }

		float pitch_rad = degrees_to_radians(pitch);
		float yaw_rad = degrees_to_radians(yaw);

		Vector3 dir(std::cos(pitch_rad) * std::cos(yaw_rad), std::sin(pitch_rad), std::cos(pitch_rad) * std::sin(yaw_rad));
		cam.rot_cam(dir);
	}

	void speed_up() {
		cam_speed = 5.0f;
	}
	
	void normal_speed() {
		cam_speed = 1.0f;
	}

	void move_forward(float dt) {
		float velocity = dt * cam_speed;
		cam.mov_cam(-velocity * cam.get_w());
	}
	void move_backward(float dt) {
		float velocity = dt * cam_speed;
		cam.mov_cam(velocity * cam.get_w());
	}
	
	void move_right(float dt) {
		float velocity = dt * cam_speed;
		cam.mov_cam(velocity * cam.get_u());
	}

	void move_left(float dt) {
		float velocity = dt * cam_speed;
		cam.mov_cam(-velocity * cam.get_u());
	}

	void move_up(float dt) {
		float velocity = dt * cam_speed;
		cam.mov_cam(velocity * cam.get_v());
	}

	void move_down(float dt) {
		float velocity = dt * cam_speed;
		cam.mov_cam(-velocity * cam.get_v());
	}
};

