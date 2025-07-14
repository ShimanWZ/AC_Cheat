// Suitable for Assault Cube version 1.3.0.2.


#include "utilities/Memory.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>


// Menu Colors
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string blue = "\033[34m";
const std::string magenta = "\033[35m";
const std::string cyan = "\033[36m";
const std::string white = "\033[37m";
const std::string reset = "\033[0m";

//offsets
constexpr auto localPlayer = 0x18AC00;
constexpr auto health = 0xEC;
constexpr auto armor = 0xF0;
constexpr auto amo = 0x11C;
constexpr auto inGunAmo = 0x140;


std::vector<std::string> gun = {
	" ,______________________________________       ",
	"|_________________,----------._ [____]  \"\"-,__  __....-----=====",
	"               (_(||||||||||||)___________/   \"\"                |",
	"                  `----------' Krogg98[ ))\" - , | ",
	"                                       \"\"    `,  _,--....___    |",
	"                                               `/           \"\"\"\""
};

std::vector<std::string> hackit = {
	"          _______  _______  _       __________________",
	"|\\     /|(  ___  )(  ____ \\| \\    /\\\\__   __/\\__   __/",
	"| )   ( || (   ) || (    \\/|  \\  / /   ) (      ) (   ",
	"| (___) || (___) || |      |  (_/ /    | |      | |   ",
	"|  ___  ||  ___  || |      |   _ (     | |      | |   ",
	"| (   ) || (   ) || |      |  ( \\ \\    | |      | |   ",
	"| )   ( || )   ( || (____/\\|  /  \\ \\___) (___   | |   ",
	"|/     \\||/     \\|(_______/|_/    \\/\\_______/   )_(   ",

};





int main() {
	//printing the logo and gun
	std::cout << std::endl;
	for (const auto& line : gun) std::cout << cyan << line << reset << std::endl;
	for (const auto& line : hackit) std::cout << cyan << line << reset << std::endl;
	std::cout << std::endl << cyan << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << reset << std::endl << std::endl;

	//getting base address and required addresses
	Memory memory{ "ac_client.exe" };
	const auto moduleBaseAddress = memory.getModuleBaseAddress("ac_client.exe");
	const auto playerAddress = memory.Read<std::uintptr_t>(moduleBaseAddress + localPlayer);
	const auto healthAddress = playerAddress + health;
	const auto armorAddress = playerAddress + armor;
	const auto amoAddress = playerAddress + amo;


	//printing instructions
	std::cout << red << "Press F1 for recieving extra health" << reset << std::endl;
	std::cout << red << "Press F2 for additional armor" << reset << std::endl;
	std::cout << red << "Press F3 for infinite amo" << reset << std::endl;


	while (TRUE) {
		if (GetAsyncKeyState(VK_F1) & 0x8000) { 
			memory.Write<int>(healthAddress, 1000); 
			std::cout << green << "Extra Health successfully added!" << std::endl;
		}
		else if (GetAsyncKeyState(VK_F2) & 0x8000) { 
			memory.Write<int>(armorAddress, 1000); 
			std::cout << green << "You're a bust now!" << std::endl;
		}
		else if (GetAsyncKeyState(VK_F3) & 0x8000) { 
			memory.Write<int>(amoAddress, 1000); 
			std::cout << green << "Rifle ammo increased!" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}