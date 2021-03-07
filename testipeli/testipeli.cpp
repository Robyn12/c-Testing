// testipeli.cpp : This file contains the 'main' function. Program execution begins and ends there.
//  By Robyn12
#include <Windows.h>
#include <iostream>
using namespace std;
int main()
{

    SetConsoleTitle(L"testipeli");
    int monster = 100;
    while (true) {
        std::cout << "Hit monster " << std::endl;
        std::cout << "Monster has hitpoints of " << monster << std::endl;
        std::cin.get();
        monster -= 5;
        if (monster <= 0) {
            std::cout << "You won the game monster is dead" << std::endl;
            std::cin.ignore();
            break;
        }
        if (monster < 50) {
            std::cout << "Monster is under 50 hp heal coming" << std::endl;
            monster = 100;
        }
    }
}

