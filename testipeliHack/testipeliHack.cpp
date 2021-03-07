// testipeliHack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <windows.h>
#include <iostream>
#include <thread>

using namespace std;
LPVOID osoitteet[8][100];
int scanRegion(int* index, int ind, int luku, HANDLE* phandle, DWORD start, DWORD end) {
	int indeksi = *index;
	int value;
	for (DWORD i = start; i < end; i+=4) // scan through memory from start till 0x1000000 
	{
		ReadProcessMemory(*phandle, (LPVOID)i, &value, sizeof(value), 0); 
		if (value == luku) {			// if value what we read is input add it to array osoitteet
			cout << std::hex << i << endl;
			osoitteet[ind][indeksi] = (LPVOID) i;
			indeksi++;
		}
	}
	osoitteet[ind][indeksi] = NULL;
	*index = indeksi;
	return indeksi;
}

int main(int argc, char* argv[])
{
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
	std::cout << pid << endl;
	SIZE_T size;
	size = sizeof(int);
	address = (DWORD)phandle;
	int indeksi = 0;
	int luku = 0;
	std::cout << std::hex << address << endl;
	std::cout << "You must hit monster once first" << endl;
	std::cout << "Give hp" << endl;
	cin >> luku;
	int indeksi1[8] = { 0,0,0,0 ,0,0,0,0};
	std::thread t1(scanRegion, &indeksi1[0], 0, luku, &phandle, address, 0x200000);
	std::thread t2(scanRegion, &indeksi1[1],1,luku, & phandle, 0x200000, 0x400000);
	std::thread t3(scanRegion ,&indeksi1[2],2, luku, &phandle, 0x400000, 0x600000);
	std::thread t4(scanRegion ,&indeksi1[3],3, luku, &phandle, 0x600000, 0x800000);
	std::thread t5(scanRegion, &indeksi1[4], 4, luku, &phandle,0x800000, 0xa00000);
	std::thread t6(scanRegion, &indeksi1[5], 5,luku, & phandle,0xa00000, 0xc00000);
	std::thread t7(scanRegion ,&indeksi1[6],6, luku, &phandle, 0xc00000, 0xe00000);
	std::thread t8(scanRegion ,&indeksi1[7],7, luku, &phandle, 0xe00000, 0x1000000);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	cout << "First Scan done" << endl;
	cout << "Give next value of hp" << endl;
	
	int c;
	cin >> c;
	LPVOID seuraavat[500];
	int indeksi2 = 0;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < indeksi1[j]; i++) {		// scan through osoitteet to see what has changed to next value

			ReadProcessMemory(phandle, (LPVOID)osoitteet[j][i], &value, sizeof(value), 0);
			if (value == c) {
				cout << std::hex << osoitteet[j][i] << endl;
				seuraavat[indeksi2] = osoitteet[j][i];	// add the address to array seuraavat
				indeksi2++;
			}
		}
	}
	cout << "Second scan done" << endl;
	cout << "Next value pls" << endl;
	cin >> c;
	LPVOID lopulliset[5];
	int indeksi3 = 0;
	for (int i = 0; i < indeksi2; i++) {		// scan through seuraavat
		ReadProcessMemory(phandle, (LPVOID)seuraavat[i], &value, sizeof(value), 0);
		if (value == c) {

			lopulliset[indeksi3] = seuraavat[i];
			indeksi3++;			// find the value what has changed 2 times in a row now
			cout << value << ":" << seuraavat[i] << "\n"; 
		}

	}
	SIZE_T bytes_written = 0;
	int arvo = 0;
	DWORD koko = 4;
	for (int i = 0; i < indeksi3; i++) {
		WriteProcessMemory(phandle, (LPVOID)lopulliset[i], &arvo, koko, &bytes_written);	// overwrite the value with 0
		cout << "written" << arvo << " to 0x" << std::hex << lopulliset[i] << endl;
	}
	std::cout << "Enter to exit..." << std::endl;

	std::cin.get();
	return 0;
}
