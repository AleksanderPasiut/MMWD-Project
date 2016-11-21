#pragma once

#include <vector>
#include <fstream>
#include "graphics.h"
#include "object.h"
#include "pipe_type.h"
#include "connection.h"

class BOARD
{
	ID2D1HwndRenderTarget* target;

	std::vector<OBJECT*> objects;
	std::vector<PIPE_TYPE*> pipe_types;
	std::vector<CONNECTION*> connections;

	OBJECT* selected;

	ID2D1SolidColorBrush* grid_brush;
	bool grid;

	BOARD(ID2D1HwndRenderTarget*, IDWriteFactory*);
	BOARD(const BOARD&) = delete;
	BOARD& operator= (const BOARD&) = delete;
	~BOARD() noexcept;

	void NewObject(OBJECT*) noexcept;
	void DeleteSelected() noexcept;
	void Clear() noexcept;

	void PaintGrid() const noexcept;
	void Paint() const noexcept;
	OBJECT* UpdateSelected(const D2D1_POINT_2F&) noexcept;

	void ManagePipeTypes() noexcept;

	void RefreshTotalObjectValues() noexcept;
	bool IsSolutionAcceptable() const noexcept;
	double SolutionCost() const noexcept;
	void ConstructAlgorithm() noexcept;
	void TabooAlgorithm() noexcept;

	void LoadFromFile(std::fstream& File);
	void SaveToFile(std::fstream& File);
	
	friend class MAIN_WINDOW;
	friend class MAIN_WINDOW_MENU;
	friend class MAIN_WINDOW_POPUP_MENU;
	friend class DIALOG_ADD_MODIFY;
	friend class IO;
	friend class PROGRAM;
	friend class FILE_MANAGER;
};