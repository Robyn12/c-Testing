// testipeliHack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <windows.h>
#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
	DWORD osoitteet[1000];
	DWORD address;
	int value; 
	DWORD pid; //This will store our Process ID, used to read/write into the memory
	HWND hwnd;
	hwnd = FindWindow(NULL, L"testipeli"); //Finds the Window called testipeli
	if (!hwnd)
	{
		cout << "Window not found!\n";
		cin.get();
	}
	GetWindowThreadProcessId(hwnd, &pid); // get pid
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid); // open process image
	if (!phandle) 
	{
		cout << "Could not get handle!\n";
		cin.get();
	}
	SIZE_T size;
	size = sizeof(int);
	address = (DWORD) phandle;
	int indeksi = 0;
	for (DWORD i = address; i < 0x1000000; i+=4) // scan through memory from start till 0x1000000 
	{
		ReadProcessMemory(phandle, (void*)i, &value, sizeof(value), 0); 
		if (value == 100) {			// if value what we read is 100 add it to array osoitteet
			osoitteet[indeksi] = i;
			indeksi++;
		}
	}
	osoitteet[indeksi] = NULL;
	cout << "First Scan done" << endl;
	cout << "Give next value of hp" << endl;
	int c;
	cin >> c;
	DWORD seuraavat[500];
	int indeksi2 = 0;
	for(int i = 0; i < indeksi; i++) {		// scan through osoitteet to see what has changed to next value
		
		ReadProcessMemory(phandle, (void*)osoitteet[i], &value, sizeof(value), 0);
		if (value == c) {
			seuraavat[indeksi2] = osoitteet[i];	// add the address to array seuraavat
			indeksi2++;
		}
	}
	cout << "toinen scanni tehty" << endl;
	cout << "Next value pls" << endl;
	cin >> c;
	DWORD lopullinen;
	for (int i = 0; i < indeksi2; i++) {		// scan through seuraavat
		ReadProcessMemory(phandle, (void*)seuraavat[i], &value, sizeof(value), 0);
		if (value == c) {
			lopullinen = seuraavat[i];			// find the value what has changed 2 times in a row now
			cout << value << ":" << seuraavat[i] << "\n"; 
		}

	}
	SIZE_T bytes_written = 0;
	int arvo = 0;
	DWORD koko = 4;
	WriteProcessMemory(phandle, (LPVOID) lopullinen,&arvo, koko, &bytes_written);	// overwrite the value with 0
	cout << "written" << arvo << " to 0x" << std::hex << lopullinen << endl;
	std::cout << "Enter to exit..." << std::endl;
	cin.ignore();
	return 0;
}
