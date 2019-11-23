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
	DWORD off1, off2, off3, off4;
	DWORD baseAddress;
	DWORD healthAddy;
	int newHealth;
	int currentHealth;
	char moduleName[] = "client_panorama.dll";
	HWND hGameWindow;
	HANDLE pHandle;

	//Get Handles
	hGameWindow = FindWindow(NULL, "Counter-Strike: Global Offensive");
	GetWindowThreadProcessId(hGameWindow, &pID);
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

	//Get Client Base Addy
	DWORD clientBase = dwGetModuleBaseAddress(_T(moduleName), pID);
	ReadProcessMemory(pHandle, (LPCVOID)(clientBase + 0x04D6B6D4), &baseAddress, sizeof(baseAddress), NULL);
	cout << "Base Addy is:" << std::hex << baseAddress << std::endl;
	ReadProcessMemory(pHandle, (LPCVOID)(baseAddress + 0x10), &off1, sizeof(off1), NULL);
	cout << "Offset 1:" << std::hex << off1 << std::endl;
	ReadProcessMemory(pHandle, (LPCVOID)(off1 + 0x24), &off2, sizeof(off2), NULL);
	cout << "Offset 2:" << std::hex << off2 << std::endl;
	ReadProcessMemory(pHandle, (LPCVOID)(off2 + 0x20), &off3, sizeof(off3), NULL);
	cout << "Offset 3:" << std::hex << off3 << std::endl;
	ReadProcessMemory(pHandle, (LPCVOID)(off3 + 0x568), &off4, sizeof(off4), NULL);
	cout << "Offset 4:" << std::hex << off4 << std::endl;
	healthAddy = off4 + 0x114;
	cout << "Final Addy:" << std::hex << healthAddy << std::endl;
	ReadProcessMemory(pHandle, (LPCVOID)(healthAddy), &currentHealth, sizeof(currentHealth), NULL);
	cout << "Health:" << currentHealth << std::endl;

	cin.get();
}

