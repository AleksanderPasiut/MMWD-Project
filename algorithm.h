#pragma once

#include <vector>
#include <fstream>
#include "object.h"
#include "pipe_type.h"
#include "connection.h"
#include "progress_bar.h"

class ALGORITHM
{
	std::vector<OBJECT*>& objects;
	std::vector<PIPE_TYPE*>& pipe_types;
	std::vector<CONNECTION*>& connections;

	PROGRESS_BAR& progressBar;

	std::wstring export_path;

	double g1;
	double g2;

	HANDLE hThread;
	double kf;
	size_t taboo_max_size;
	size_t best_iteration;
	size_t max_iterations;

	ALGORITHM(std::vector<OBJECT*>& objects, std::vector<PIPE_TYPE*>& pipe_types, std::vector<CONNECTION*>& connections, PROGRESS_BAR& progressBar);
	ALGORITHM(const ALGORITHM&) = delete;
	ALGORITHM& operator= (const ALGORITHM&) = delete;

	void RefreshTotalObjectValues(const std::vector<CONNECTION*>&) noexcept;
	void RefreshObjectValues(double& old_source, double& old_target, OBJECT* source, OBJECT* target, double capacity) noexcept;
	void RestoreObjectValues(OBJECT* source, OBJECT* target, double old_source, double old_target) noexcept;
	bool IsSolutionAcceptable() const noexcept;
	double SolutionCost(const std::vector<CONNECTION*>&) const noexcept;
	void ClearConnections() noexcept;

	double PipeCapacityToPrice(double capacity) const noexcept;
	double func_cT() const noexcept;
	double OutOfAcceptance() const noexcept;
	double OutOfAcceptance(const CONNECTION& connection, double pipe_capacity_change) const noexcept;
	void Core() noexcept;
	
	void ApplyDefaultExportPath(std::wstring&) noexcept;
	void LoadFromFile(std::fstream& File);
	void SaveToFile(std::fstream& File);

	friend class BOARD;
	friend DWORD WINAPI AlgorithmThreadProc(void*) noexcept;
	friend DWORD WINAPI TestThreadProc(void*) noexcept;
};