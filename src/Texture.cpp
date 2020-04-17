#include "include/Texture.hpp"

Texture::Texture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture::~Texture() {
	free();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, string path) {
	free();
	SDL_Texture* loadedTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		cout << "Couldn't load file: " << path.c_str() << " IMG_Load error: " << IMG_GetError();
	}
	else {
		loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (loadedTexture == NULL) {
			cout << "Couldn't create texture from surface! " << SDL_GetError();
		}
		mWidth = loadedSurface->w;
		mHeight = loadedSurface->h;
		SDL_FreeSurface(loadedSurface);
	}
	mTexture = loadedTexture;
	return mTexture != NULL;
}

bool Texture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, string text, SDL_Color color){
	free();
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (textSurface == NULL) {
		cout << "Couldn't render text surface! " << TTF_GetError();
	}
	else {
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL) {
			cout << "Couldn't create texture from surface! " << SDL_GetError();
		}
		else {
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}
	return mTexture != NULL;
}

void Texture::free() {
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
	SDL_Rect renderRect = { x, y, mWidth, mHeight };
	if (clip != NULL) {
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}
	SDL_RenderCopy(renderer, mTexture, clip, &renderRect);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void BackgroundTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
	SDL_RenderCopy(renderer, getTexture(), clip, NULL);
}

void TetrominoTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
	SDL_Rect renderRect = { x, y, TILE_RENDER_SIZE, TILE_RENDER_SIZE };
	SDL_RenderCopy(renderer, getTexture(), clip, &renderRect);
}

SDL_Texture* Texture::getTexture() { return mTexture; }
int Texture::getWidth() { return mWidth; }
int Texture::getHeight() { return mHeight; }