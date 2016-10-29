#pragma once

#include "graphics.h"

class VIEW_MANAGEMENT
{
	ID2D1HwndRenderTarget* target;

	D2D1::Matrix3x2F logicToPhysic;
	D2D1::Matrix3x2F physicToLogic;
	D2D1_POINT_2F shift;
	float zoom;
	float zoom_speed;

	bool active;
	POINT click;
	D2D1_POINT_2F init_shift;

	VIEW_MANAGEMENT();
	VIEW_MANAGEMENT(const VIEW_MANAGEMENT&) = delete;
	VIEW_MANAGEMENT& operator=(const VIEW_MANAGEMENT&) = delete;

	void SetTarget(ID2D1HwndRenderTarget*) noexcept;
	void UpdateMatrix() noexcept;
	void MovingStart(WPARAM, LPARAM) noexcept;
	bool MovingPerform(WPARAM, LPARAM) noexcept;
	void MovingEnd(WPARAM, LPARAM) noexcept;
	void Zooming(WPARAM, LPARAM) noexcept;

	friend class MAIN_WINDOW;
};