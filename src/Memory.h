#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <string_view>

class Memory {
private:
	std::uintptr_t processID = 0;
	void* processHandle = nullptr;

public:
	Memory(const std::string_view processName) noexcept {
		::PROCESSENTRY32 entry = {};
		entry.dwSize = sizeof(::PROCESSENTRY32);
		
		// take snapshot
		const auto processSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		//find desired game
		while (::Process32Next(processSnapshot, &entry)) {
			if (!processName.compare(entry.szExeFile)) {
				processID = entry.th32ProcessID;
				//attach to game using privilages
				processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
				break;
			}
		}
		if (processSnapshot) {
			::CloseHandle(processSnapshot);
		}
	}

	~Memory() {
		// close the handles
		if (processHandle) {
			::CloseHandle(processHandle);
		}
	}

	const std::uintptr_t getModuleBaseAddress(const std::string_view moduleName) const noexcept {
		::MODULEENTRY32 entry = {};
		entry.dwSize = sizeof(::MODULEENTRY32);

		// take snapshot
		const auto processSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
		std::uintptr_t moduleBaseAddress = 0;
		//find desired game
		while (Module32Next(processSnapshot, &entry)) {
			if (!moduleName.compare(entry.szModule)) {
				moduleBaseAddress = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}
		if (processSnapshot) {
			::CloseHandle(processSnapshot);
		}
		return moduleBaseAddress;
	};

	//read memory
	template <typename T>
	constexpr const T Read(const std::uintptr_t& address) const noexcept {
		T value = {};
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}
	//write memory
	template <typename T>
	constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept {
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};