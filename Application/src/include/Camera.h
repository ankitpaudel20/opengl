#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



struct Euler_angles { float  m_roll, m_yaw, m_pitch; };


struct Camera {
	glm::vec3 Camera_Facing_Direction;
	glm::vec3 Camera_Position;
	glm::vec3 Camera_Up;
	float Camera_Target_distance;
	Euler_angles angles;


	Camera() : Camera_Facing_Direction(0.0f, 0.0f, -1.0f), Camera_Position(0.0f, 0.0f, 0.0f), Camera_Up(0.0f, 1.0f, 0.0f), Camera_Target_distance(10),
		angles({0,0,0})	{}

	Camera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& dir)
	    : Camera_Target_distance(10),Camera_Facing_Direction(glm::normalize(dir)),Camera_Up(glm::normalize(up)),Camera_Position(pos)
	{
		angles.m_roll = std::atan2(dir.x, dir.y);
		angles.m_yaw = std::atan2(dir.z, dir.x);
		angles.m_pitch = std::atan2(dir.y, dir.z);
	}

	Camera(const glm::vec3& pos, const glm::vec3& up, float roll, float yaw, float pitch)
	    :Camera_Target_distance(10), angles({glm::radians(roll),glm::radians(yaw),glm::radians(pitch)}),Camera_Up(glm::normalize(up)),Camera_Position(pos)
	{
		Camera_Facing_Direction.x = std::sin(angles.m_roll) * std::sin(angles.m_pitch) - std::cos(angles.m_roll) * std::sin(angles.m_yaw) * std::cos(angles.m_pitch);
		Camera_Facing_Direction.y = std::cos(angles.m_roll) * std::sin(angles.m_pitch) + std::sin(angles.m_roll) * std::sin(angles.m_yaw) * std::cos(angles.m_pitch);
		Camera_Facing_Direction.z = -std::cos(angles.m_yaw) * std::cos(angles.m_pitch);
		Camera_Facing_Direction = glm::normalize(Camera_Facing_Direction);
	}

	void ChangeDir(const glm::vec3& dir)
	{
		Camera_Facing_Direction = glm::normalize(dir);
		angles.m_roll = std::atan2(dir.y, dir.x);
		angles.m_pitch = std::atan2(dir.z, dir.y);
		angles.m_yaw = std::atan2(dir.x, dir.z);
	}

	void DelRoll(float angle_in_deg)
	{
		angles.m_roll += glm::radians(angle_in_deg);

		Camera_Facing_Direction.x = std::sin(angles.m_roll) * std::sin(angles.m_pitch) - std::cos(angles.m_roll) * std::sin(angles.m_yaw) * std::cos(angles.m_pitch);
		Camera_Facing_Direction.y = std::cos(angles.m_roll) * std::sin(angles.m_pitch) + std::sin(angles.m_roll) * std::sin(angles.m_yaw) * std::cos(angles.m_pitch);

		Camera_Up.x = std::cos(angles.m_roll) * std::sin(angles.m_yaw) * std::sin(angles.m_pitch) + std::sin(angles.m_roll) * std::cos(angles.m_pitch);
		Camera_Up.y = -std::sin(angles.m_roll) * std::sin(angles.m_yaw) * std::sin(angles.m_pitch) + std::cos(angles.m_roll) * std::cos(angles.m_pitch);

	}

	void DelYaw(float angle_in_deg)
	{

		angles.m_yaw += glm::radians(angle_in_deg);

		Camera_Facing_Direction.x = std::sin(angles.m_roll) * std::sin(angles.m_pitch) - std::cos(angles.m_roll) * std::sin(angles.m_yaw) * std::cos(angles.m_pitch);
		Camera_Facing_Direction.z = -std::cos(angles.m_yaw) * std::cos(angles.m_pitch);

		Camera_Up.x = std::cos(angles.m_roll) * std::sin(angles.m_yaw) * std::sin(angles.m_pitch) + std::sin(angles.m_roll) * std::cos(angles.m_pitch);
		Camera_Up.z = std::cos(angles.m_yaw) * std::sin(angles.m_pitch);

	}

	void DelPitch(float angle_in_deg) {
		angles.m_pitch += glm::radians(angle_in_deg);
		if (angles.m_pitch > glm::radians(89.99f))
			angles.m_pitch = glm::radians(89.99f);
		if (angles.m_pitch < glm::radians(-89.99f))
			angles.m_pitch = glm::radians(-89.99f);
		Camera_Facing_Direction.x = std::sin(angles.m_roll) * std::sin(angles.m_pitch) - std::cos(angles.m_roll) * std::sin(angles.m_yaw) * std::cos(angles.m_pitch);
		Camera_Facing_Direction.y = std::cos(angles.m_roll) * std::sin(angles.m_pitch) + std::sin(angles.m_roll) * std::sin(angles.m_yaw) * std::cos(angles.m_pitch);
		Camera_Facing_Direction.z = -std::cos(angles.m_yaw) * std::cos(angles.m_pitch);

		Camera_Up.x = std::cos(angles.m_roll) * std::sin(angles.m_yaw) * std::sin(angles.m_pitch) + std::sin(angles.m_roll) * std::cos(angles.m_pitch);
		Camera_Up.y = -std::sin(angles.m_roll) * std::sin(angles.m_yaw) * std::sin(angles.m_pitch) + std::cos(angles.m_roll) * std::cos(angles.m_pitch);
		Camera_Up.z = std::cos(angles.m_yaw) * std::sin(angles.m_pitch);
	}

	const Euler_angles& getAngles() { return angles; }


};
