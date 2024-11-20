#include "imgui.h"
#include <string>

namespace Renderer {
	inline void Rectangle(ImDrawList* drawlist, ImVec2 pos, ImVec2 size, ImColor color, float rounding = 0.f) {
		drawlist->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), color, rounding);
	};

	inline void FilledRectangle(ImDrawList* drawlist, ImVec2 pos, ImVec2 size, ImColor color, float rounding = 0.f) {
		drawlist->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), color, rounding);
	};

	inline void Gradient(ImDrawList* drawlist, ImVec2 pos, ImVec2 size, ImColor col_upr_left, ImColor col_upr_right, ImColor col_bot_right, ImColor col_bot_left) {
		drawlist->AddRectFilledMultiColor(pos, ImVec2(pos.x + size.x, pos.y + size.y), col_upr_left, col_upr_right, col_bot_right, col_bot_left);
	}

	inline void Text(ImDrawList* drawlist, ImVec2 pos, std::string text, ImColor color, ImColor outline, ImFont* font = NULL, float size = 0.0f, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL) {
		if (outline.operator ImU32() != IM_COL32(0, 0, 0, 0)) {
			drawlist->AddText(font, size, ImVec2(pos.x + 1, pos.y + 1), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
			drawlist->AddText(font, size, ImVec2(pos.x - 1, pos.y - 1), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
			drawlist->AddText(font, size, ImVec2(pos.x - 1, pos.y), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
			drawlist->AddText(font, size, ImVec2(pos.x, pos.y - 1), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
			drawlist->AddText(font, size, ImVec2(pos.x + 1, pos.y), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
			drawlist->AddText(font, size, ImVec2(pos.x, pos.y + 1), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
			drawlist->AddText(font, size, ImVec2(pos.x - 1, pos.y + 1), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
			drawlist->AddText(font, size, ImVec2(pos.x + 1, pos.y - 1), outline, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
		}

		drawlist->AddText(font, size, pos, color, text.c_str(), NULL, wrap_width, cpu_fine_clip_rect);
	}
};