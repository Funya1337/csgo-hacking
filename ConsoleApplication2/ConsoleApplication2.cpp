#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>

using namespace std;

DWORD dwGetModuleBaseAddress(TCHAR *lpszModuleName, DWORD pID) {
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32))
	{
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) {
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32));
	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}

int main() {
	DWORD pID;
	DWORD off1, off2, off3, off4, off5, off6;
	DWORD baseAddress;
	DWORD clientAddress;
	DWORD FinalyAddy;
	DWORD ClientFinalyAddy;
	int BunnyHop;
	int fj;
	int Forcejump;
	char moduleName[] = "engine.dll";
	char moduleNameClient[] = "client_panorama.dll";
	HWND hGameWindow;
	HANDLE pHandle;

	hGameWindow = FindWindow(NULL, "Counter-Strike: Global Offensive");
	GetWindowThreadProcessId(hGameWindow, &pID);
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	cout << "BunnyHop is started!" << endl;
	for (;;)
	{
		DWORD clientBase = dwGetModuleBaseAddress(_T(moduleName), pID);
		ReadProcessMemory(pHandle, (LPCVOID)(clientBase + 0x007E8C60), &baseAddress, sizeof(baseAddress), NULL);
		ReadProcessMemory(pHandle, (LPCVOID)(baseAddress + 0x140), &off1, sizeof(off1), NULL);
		ReadProcessMemory(pHandle, (LPCVOID)(off1 + 0x0), &off2, sizeof(off2), NULL);
		ReadProcessMemory(pHandle, (LPCVOID)(off2 + 0x8C), &off3, sizeof(off3), NULL);
		ReadProcessMemory(pHandle, (LPCVOID)(off3 + 0x154), &off4, sizeof(off4), NULL);
		ReadProcessMemory(pHandle, (LPCVOID)(off4 + 0x84), &off5, sizeof(off5), NULL);
		ReadProcessMemory(pHandle, (LPCVOID)(off5 + 0x60), &off6, sizeof(off6), NULL);
		FinalyAddy = off6 + 0x104;
		ReadProcessMemory(pHandle, (LPCVOID)(FinalyAddy), &BunnyHop, 4, NULL);
		if (GetAsyncKeyState(VK_SPACE) && BunnyHop == 257)
		{
			fj = 5;
			DWORD forceJumpBase = dwGetModuleBaseAddress(_T(moduleNameClient), pID);
			ReadProcessMemory(pHandle, (LPCVOID)(forceJumpBase + 0x002C1788), &clientAddress, sizeof(clientAddress), NULL);
			ClientFinalyAddy = clientAddress + 0xA0;
			ReadProcessMemory(pHandle, (LPCVOID)(ClientFinalyAddy), &Forcejump, sizeof(int), NULL);
			WriteProcessMemory(pHandle, (LPVOID)(ClientFinalyAddy), &fj, sizeof(int), 0);
		}
		else
		{
			fj = 4;
			DWORD forceJumpBase = dwGetModuleBaseAddress(_T(moduleNameClient), pID);
			ReadProcessMemory(pHandle, (LPCVOID)(forceJumpBase + 0x002C1788), &clientAddress, sizeof(clientAddress), NULL);
			ClientFinalyAddy = clientAddress + 0xA0;
			ReadProcessMemory(pHandle, (LPCVOID)(ClientFinalyAddy), &Forcejump, sizeof(int), NULL);
			WriteProcessMemory(pHandle, (LPVOID)(ClientFinalyAddy), &fj, sizeof(int), 0);
		}
	}
}

