#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <utility>

namespace JJ0o0::TerrainGeneration::Utils {
void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                int x, int y, SDL_Color color);
void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                float x, float y, SDL_Color color);
void renderTextCentered(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &text, int screenWidth,
                        int screenHeight, SDL_Color color);
std::pair<float, float> getTextSize(TTF_Font *font, const std::string &text);
} // namespace JJ0o0::TerrainGeneration::Utils