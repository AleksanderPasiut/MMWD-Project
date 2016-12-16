#include <string>
#include <fstream>
#include "file_manager.h"

wchar_t FILE_MANAGER::fileOpened[MAX_PATH_LENGTH];
HWND FILE_MANAGER::hwnd = 0;
BOARD* FILE_MANAGER::board = 0;
VIEW_MANAGEMENT* FILE_MANAGER::viewManagement = 0;
MAIN_WINDOW_MENU* FILE_MANAGER::mainWindowMenu = 0;

OPENFILENAMEW FILE_MANAGER::openFileName;

void FILE_MANAGER::GetFilePathFromCommandLine() noexcept
{
	wchar_t* command = GetCommandLineW();
	int argc;
	wchar_t** argv = CommandLineToArgvW(command, &argc);

	if (argc < 2)
		return;

	memcpy(fileOpened, argv[1], lstrlenW(argv[1])*sizeof(wchar_t));
	ApplyDefaultExportPath();
	return;
}
void FILE_MANAGER::ApplyDefaultExportPath() noexcept
{
	using namespace std;
	wstring path(fileOpened);
	auto it = path.end()-1;
	while(*it != L'\\')
		--it;
	path.erase(it+1, path.end());
	path += wstring(L"export.txt");
	board->ApplyDefaultExportPath(path);
}
void FILE_MANAGER::UpdateMainWindowText() noexcept
{
	if (fileOpened[0])
	{
		unsigned i = 0;
		for (;fileOpened[i] != 0; i++);
		for (;fileOpened[i] != L'.'; i--);
		unsigned dot = i;
		for (;fileOpened[i] != L'\\'; i--);
		std::wstring text(fileOpened+i+1, dot-i-1);
		text += L" - MMWD Project Simulator & Solver";

		SetWindowTextW(hwnd, text.c_str());
	}
	else SetWindowTextW(hwnd, L"MMWD Project Simulator & Solver");
}
BOOL FILE_MANAGER::OpenFileDialog() noexcept
{
	openFileName.lpstrTitle = L"Otwórz konfiguracjê";
	BOOL ret = GetOpenFileNameW(&openFileName);
	if (ret)
		ApplyDefaultExportPath();
	return ret;
}
void FILE_MANAGER::ApplyOpenedFile() noexcept
{
	if (!fileOpened[0])
		return;

	try
	{
		using namespace std;

		fstream File(fileOpened, fstream::in);

		if (!File)
			throw 0;

		char tag[12];
		File.read(tag, 12);
		
		if (strcmp(tag, "MMWD1.0file"))
			throw 0;

		File.read(reinterpret_cast<char*>(&board->grid), sizeof(bool));
		File.read(reinterpret_cast<char*>(&OBJECT::info), sizeof(bool));
		File.read(reinterpret_cast<char*>(&CONNECTION::info), sizeof(bool));
		mainWindowMenu->Update();

		File.read(reinterpret_cast<char*>(&viewManagement->zoom), sizeof(float));
		File.read(reinterpret_cast<char*>(&viewManagement->shift), sizeof(D2D1_POINT_2F));
		viewManagement->UpdateMatrix();

		board->LoadFromFile(File);

		UpdateMainWindowText();
	}
	catch(...) { MessageBoxW(hwnd, L"B³¹d odczytu konfiguracji", L"B³¹d", MB_OK); }
}
BOOL FILE_MANAGER::SaveFileDialog() noexcept
{
	openFileName.lpstrTitle = L"Zapisz konfiguracjê";
	return GetSaveFileNameW(&openFileName);
}
void FILE_MANAGER::SaveToFile() noexcept
{
	try
	{
		using namespace std;

		fstream File(fileOpened, fstream::out);

		if (!File)
			throw 0;

		File.write("MMWD1.0file", 12);
		File.write(reinterpret_cast<const char*>(&board->grid), sizeof(bool));
		File.write(reinterpret_cast<const char*>(&OBJECT::info), sizeof(bool));
		File.write(reinterpret_cast<const char*>(&CONNECTION::info), sizeof(bool));

		File.write(reinterpret_cast<const char*>(&viewManagement->zoom), sizeof(float));
		File.write(reinterpret_cast<const char*>(&viewManagement->shift), sizeof(D2D1_POINT_2F));

		board->SaveToFile(File);

		UpdateMainWindowText();
	}
	catch(...) { MessageBoxW(hwnd, L"B³¹d zapisu konfiguracji.", L"B³¹d", MB_OK); }
}

void FILE_MANAGER::PreOpen() noexcept
{
	HWND target = FindWindow(L"SimulatorMainWindowClass", 0);
	wchar_t* command_line = GetCommandLineW();
	int command_line_length = int(wcslen(command_line));
	HANDLE hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, command_line_length*sizeof(wchar_t), L"SimulatorFileShared");
	
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return;

	void* buffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, command_line_length*sizeof(wchar_t));
	memcpy(buffer, command_line, command_line_length*sizeof(wchar_t));
	SendMessage(target, WM_TRANSFER, static_cast<WPARAM>(command_line_length), 0);

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
}
void FILE_MANAGER::InitFileManager(HWND hwnd, BOARD* board, VIEW_MANAGEMENT* viewManagement, MAIN_WINDOW_MENU* mainWindowMenu)
{
	FILE_MANAGER::fileOpened[0] = 0;
	FILE_MANAGER::hwnd = hwnd;
	FILE_MANAGER::board = board;
	FILE_MANAGER::viewManagement = viewManagement;
	FILE_MANAGER::mainWindowMenu = mainWindowMenu;

	ZeroMemory(&openFileName, sizeof(OPENFILENAMEW));
	openFileName.lStructSize = sizeof(OPENFILENAMEW);
	openFileName.hwndOwner = hwnd;
	openFileName.lpstrFile = fileOpened;
	openFileName.lpstrFile[0] = 0;
	openFileName.nMaxFile = MAX_PATH_LENGTH;
	openFileName.lpstrFilter = L"*.mmwd\0*.mmwd";
	openFileName.nFilterIndex = 1;
	openFileName.lpstrDefExt = L".mmwd";
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	UpdateMainWindowText();
	GetFilePathFromCommandLine();
	ApplyOpenedFile();
}
void FILE_MANAGER::NewFile() noexcept
{
	board->Clear();

	viewManagement->zoom = 1.0f;
	viewManagement->shift = D2D1::Point2F();
	viewManagement->UpdateMatrix();

	board->grid = true;
	OBJECT::info = true;
	CONNECTION::info = true;
	mainWindowMenu->Update();
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);

	fileOpened[0] = 0;
	UpdateMainWindowText();
}
void FILE_MANAGER::OpenFile() noexcept
{
	if (OpenFileDialog())
	{
		ApplyDefaultExportPath();
		ApplyOpenedFile();
	}
}
void FILE_MANAGER::OpenTransferFile(WPARAM wParam) noexcept
{
	HANDLE hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, L"SimulatorFileShared");

	if (!hMapFile)
		return;

	void* buffer = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, static_cast<int>(wParam)*sizeof(wchar_t));

	if (!buffer)
		return;

	wchar_t* command = reinterpret_cast<wchar_t*>(buffer);
	int argc;
	wchar_t** argv = CommandLineToArgvW(command, &argc);

	if (argc < 2)
		return;

	memcpy(fileOpened, argv[1], lstrlenW(argv[1])*sizeof(wchar_t));
	ApplyDefaultExportPath();

	ApplyOpenedFile();

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
	return;
}
void FILE_MANAGER::SaveFile() noexcept
{
	if (fileOpened[0] || SaveFileDialog())
	{
		ApplyDefaultExportPath();
		SaveToFile();
	}
}
void FILE_MANAGER::SaveFileAs() noexcept
{
	if (SaveFileDialog())
	{
		ApplyDefaultExportPath();
		SaveToFile();
	}
}
void FILE_MANAGER::FreeFileManager() noexcept
{
	
}