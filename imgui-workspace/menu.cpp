#include "menu.h"

void Menu::DrawWindow(std::string name) {
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
	ImDrawList* drawlist = ImGui::GetWindowDrawList();
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();

	float innerwidth = 2.0f; // goofy number
	static float l_change_time = 0.f, l_change_interval = 0.15f;

	// main window
	Renderer::FilledRectangle(drawlist, ImVec2(window_pos.x, window_pos.y), ImVec2(window_size.x, window_size.y), Menu::Colors::White);
	Renderer::FilledRectangle(drawlist, ImVec2(window_pos.x, window_pos.y), ImVec2(window_size.x, 10), Menu::Colors::Navy);

	// 1st inline
	Renderer::FilledRectangle(drawlist, ImVec2(window_pos.x + 2, window_pos.y + 12), ImVec2(innerwidth, window_size.y - 14), Menu::Colors::Navy, 0.f);
	Renderer::FilledRectangle(drawlist, ImVec2(window_pos.x + window_size.x - innerwidth - 2, window_pos.y + 12), ImVec2(innerwidth, window_size.y - 14), Menu::Colors::Navy, 0.f);
	Renderer::Rectangle(drawlist, ImVec2(window_pos.x + 2, window_pos.y + 12), ImVec2(window_size.x - 4, window_size.y - 14), Menu::Colors::Navy, 0.f);

	// 2nd inline
	Renderer::FilledRectangle(drawlist, ImVec2(window_pos.x + 5, window_pos.y + 14), ImVec2(innerwidth, window_size.y - 18), Menu::Colors::Navy, 0.f);
	Renderer::FilledRectangle(drawlist, ImVec2(window_pos.x + window_size.x - innerwidth - 5, window_pos.y + 14), ImVec2(innerwidth, window_size.y - 18), Menu::Colors::Navy, 0.f);
	Renderer::Rectangle(drawlist, ImVec2(window_pos.x + 5, window_pos.y + 14), ImVec2(window_size.x - 10, window_size.y - 18), Menu::Colors::Navy, 0.f);

	// window label
	ImGui::SetCursorPos({ 0, 2 });
	Menu::Label(get_local_time() + "  FPS: " + std::to_string(int(ImGui::GetIO().Framerate)) + " Lc1:00000000", false);

	ImVec2 text_size = ImGui::CalcTextSize("MENU");

	// menu blinking label
	Renderer::FilledRectangle(drawlist, ImVec2(window_pos.x + 8, window_pos.y + 10), ImVec2(text_size.x, text_size.y), Menu::blink_switch ? Menu::Colors::Navy : Menu::Colors::White);
	ImGui::SetCursorPos({ 7, 10 });
	Menu::Label("MENU", false, Menu::blink_switch ? Menu::Colors::White : Menu::Colors::Navy);

	// cheat name label
	static std::string title = "TempleHook......";

	l_change_time += ImGui::GetIO().DeltaTime;
	if (l_change_time >= l_change_interval) {
		std::rotate(std::begin(title), std::next(std::begin(title)), std::end(title));
		l_change_time = 0.f;
	}
	ImGui::SetCursorPos({ 223, 10 });
	Menu::Label(title);

	ImGui::SetCursorPos({ 0, 19 });

}

bool Menu::Toggle(std::string name, bool* val) {
	ImGuiWindow* Window = ImGui::GetCurrentWindow();
	ImGuiID ID = Window->GetID(name.c_str());

	std::string checkbox_text = "[" + std::string(*val ? "x" : " ") + "] " + name;

	ImVec2 TextSize = ImGui::CalcTextSize(checkbox_text.c_str());
	ImVec2 CursorPos = Window->DC.CursorPos;

	ImVec2 RectMin = CursorPos;
	ImVec2 RectMax = CursorPos + TextSize; 
	const ImRect Rect(RectMin, RectMax);

	ImGui::ItemSize(Rect, 0.0f);
	if (!ImGui::ItemAdd(Rect, ID))
		return false;

	bool Hovered, Held;
	bool Pressed = ImGui::ButtonBehavior(Rect, ID, &Hovered, &Held, NULL);

	if (Pressed) {
		*val = !(*val);
		ImGui::MarkItemEdited(ID);
	}
	ImGui::RenderNavHighlight(Rect, ID);

	static float cur_time = 0.0f, interval_per_tick = 0.03f;
	cur_time += ImGui::GetIO().DeltaTime;
	auto blink_switch = static_cast<int>(std::floor((cur_time * 0.075f) / interval_per_tick)) % 2;

	if (Hovered)
		Window->DrawList->AddRectFilled(RectMin, RectMax, Hovered ? (blink_switch ? Menu::Colors::White : Menu::Colors::Navy) : Menu::Colors::White);

	ImVec2 TextPosition = CursorPos;
	Renderer::Text(Window->DrawList, TextPosition, checkbox_text, Hovered ? (blink_switch ? Menu::Colors::Navy : Menu::Colors::White) : Menu::Colors::Navy, Menu::Colors::Clear);

	return Pressed;
}

bool Menu::Label(std::string name, bool background, ImColor color) {
	ImGuiWindow* Window = ImGui::GetCurrentWindow();

	ImGuiID ID = Window->GetID(name.c_str());
	ImVec2 TextSize = ImGui::CalcTextSize(name.c_str());

	float Size = ImGui::CalcItemWidth();

	ImVec2 CursorPos = Window->DC.CursorPos;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& Style = g.Style;

	ImVec2 RectMin = CursorPos;
	ImVec2 RectMax = CursorPos + TextSize;
	const ImRect Rect(RectMin, RectMax);

	ImGui::ItemSize(Rect, Style.FramePadding.y);
	if (!ImGui::ItemAdd(Rect, ID))
		return false;

	ImVec2 LabelPosition = ImVec2(Rect.Min.x, Rect.Min.y);

	if (background)
		Window->DrawList->AddRectFilled(RectMin, RectMax, Menu::Colors::Navy);

	Renderer::Text(Window->DrawList, LabelPosition, name, color, Menu::Colors::Clear);

	return true;
}

bool Menu::Tab(std::string name) {
	static ImGuiID lastPressedTabID = 0;
	ImGuiWindow* Window = ImGui::GetCurrentWindow();

	ImGuiID ID = Window->GetID(name.c_str());
	ImVec2 TextSize = ImGui::CalcTextSize(name.c_str());

	float rectWidth = 8.f; // Tab width
	float rectHeight = TextSize.y * name.size(); 

	ImVec2 CursorPos = Window->DC.CursorPos;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& Style = g.Style;

	ImVec2 RectMin = CursorPos;
	ImVec2 RectMax = CursorPos + ImVec2(rectWidth, rectHeight);
	const ImRect Rect(RectMin, RectMax);
	ImGui::ItemSize(Rect, Style.FramePadding.y);
	if (!ImGui::ItemAdd(Rect, ID))
		return false;

	bool Hovered, Held;
	bool Pressed = ImGui::ButtonBehavior(Rect, ID, &Hovered, &Held, NULL);

	Window->DrawList->AddRectFilled(RectMin, RectMax, (ID == lastPressedTabID || Hovered) ? (blink_switch ? Menu::Colors::White : Menu::Colors::Navy) : Menu::Colors::White);

	float offsetY = 0.0f;
	for (const char& c : name) {
		ImVec2 charPos = ImVec2(Rect.Min.x, Rect.Min.y + offsetY);
		Window->DrawList->AddText(charPos, (ID == lastPressedTabID || Hovered) ? (blink_switch ? Menu::Colors::Navy : Menu::Colors::White) : Menu::Colors::Navy, std::string(1, c).c_str());
		offsetY += TextSize.y;
	}

	if (Pressed) {
		lastPressedTabID = ID; 
		ImGui::MarkItemEdited(ID);
		return true; 
	}

	ImGui::RenderNavHighlight(Rect, ID);

	return false; 
}

void Menu::Watermark(std::string text){
	ImVec2 TextSize = ImGui::CalcTextSize(text.c_str());
	ImVec2 WindowSize = TextSize;

	ImGui::SetNextWindowSize(WindowSize);
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);

	ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

	ImDrawList* WindowDrawList = ImGui::GetBackgroundDrawList();
	ImVec2 WindowPosition = ImGui::GetWindowPos();

	Renderer::FilledRectangle(WindowDrawList, WindowPosition, WindowSize, Menu::Colors::Navy);

	Renderer::Text(WindowDrawList, WindowPosition, text.c_str(), Menu::Colors::White, Menu::Colors::Clear);

	ImGui::End();
}