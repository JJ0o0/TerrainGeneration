#include <TerrainGeneration/utilities/UI.hpp>

namespace JJ0o0::TerrainGeneration::Utils {
void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                int x, int y, SDL_Color color) {
  SDL_Surface *surface =
      TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  SDL_FRect dst = {(float)x, (float)y, w, h};
  SDL_RenderTexture(renderer, texture, NULL, &dst);

  SDL_DestroySurface(surface);
  SDL_DestroyTexture(texture);
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                float x, float y, SDL_Color color) {
  SDL_Surface *surface =
      TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  SDL_FRect dst = {x, y, w, h};
  SDL_RenderTexture(renderer, texture, NULL, &dst);

  SDL_DestroySurface(surface);
  SDL_DestroyTexture(texture);
}

void renderTextCentered(SDL_Renderer *renderer, TTF_Font *font,
                        const std::string &text, int screenWidth,
                        int screenHeight, SDL_Color color) {
  std::pair<float, float> textSize = getTextSize(font, text);
  renderText(renderer, font, text, (float)screenWidth / 2 - textSize.first / 2,
             (float)screenHeight / 2 - textSize.second / 2, color);
}

std::pair<float, float> getTextSize(TTF_Font *font, const std::string &text) {
  int width, height;
  TTF_GetStringSize(font, text.c_str(), text.length(), &width, &height);

  return {width, height};
}
} // namespace JJ0o0::TerrainGeneration::Utils