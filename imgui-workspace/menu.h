#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "renderer.hpp"

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <chrono>
#include <string>
#include <ctime>
#include <cmath>

#include <math.h>

using namespace ImGui;

namespace Menu {
	inline bool blink_switch;

	namespace Colors {
		inline ImColor Clear(0, 0, 0, 0);
		inline ImColor Black(0, 0, 0);
		inline ImColor White(255, 255, 255);
		inline ImColor Navy(0, 0, 170);
	};

	void DrawWindow(std::string name);
	bool Toggle(std::string name, bool* val);
	bool Label(std::string name, bool background = true, ImColor color = Menu::Colors::White);
	bool Tab(std::string name);
	void Watermark(std::string text);
};

inline std::string get_local_time() {
	auto now = std::chrono::system_clock::now();
	std::time_t ttn = std::chrono::system_clock::to_time_t(now);
	std::tm lt;
	localtime_s(&lt, &ttn);

	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%a %b  %d %Y %H:%M:%S", &lt); // Example: "Fri, Sep 25, 2020 16:45:05"
	return std::string(buffer);
}