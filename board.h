#pragma once

#include <vector>
#include <fstream>
#include "graphics.h"
#include "object.h"
#include "pipe_type.h"
#include "connection.h"
#include "progress_bar.h"

class BOARD
{
	ID2D1HwndRenderTarget* target;

	std::vector<OBJECT*> objects;
	std::vector<PIPE_TYPE*> pipe_types;
	std::vector<CONNECTION*> connections;

	double g1;	// pumping system cost parameters
	double g2;

	HANDLE algorithmThread;
	double kf;	// taboo algorithm parameters
	size_t taboo_max_size;
	size_t max_iterations;

	OBJECT* selected;

	ID2D1SolidColorBrush* grid_brush;
	bool grid;

	PROGRESS_BAR progressBar;

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
	void ManagePumpingSystemCost() noexcept;
	void LaunchTabooAlgorithm() noexcept;
	friend DWORD WINAPI AlgorithmThreadProc(void*) noexcept;
	void BreakAlgorithm() noexcept;
	void PresentSolutionDetails() noexcept;

	void RefreshTotalObjectValues(const std::vector<CONNECTION*>&) noexcept;
	bool IsSolutionAcceptable() const noexcept;
	double SolutionCost(const std::vector<CONNECTION*>&) const noexcept;
	void ClearConnections() noexcept;

	double PipeCapacityToPrice(double capacity) const noexcept;
	double func_cT() const noexcept;
	double OutOfAcceptance() const noexcept;
	bool InTabooList(const std::vector<MOVE*>&, const MOVE& move) const noexcept;
	void TabooAlgorithmCore() noexcept;

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