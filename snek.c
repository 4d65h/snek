#include <stdio.h>
#include <SDL3/SDL.h>
#include "snek.h"
#include <stdlib.h>
#include <time.h>


const double version = 0.1;
bool isRunning = true;

bool showLostMessage = false;

uint32_t snekLength = 1;
struct SnekPiece* snek;
enum SnekDirection snekDirection = SNEK_RIGHT;
struct MapSize mapSize;
size_t max_score = 0;

bool* appleMap;
bool* playerColisionMap;

float refreshRate = 165;
float snekSpeed;

void keyHandler(SDL_KeyboardEvent* key_event) {
    switch (key_event->key) {
        case SDLK_ESCAPE:
            isRunning = false;
            break;
        case SDLK_UP:
            snekDirection = SNEK_UP;
            break;
        case SDLK_DOWN:
            snekDirection = SNEK_DOWN;
            break;
        case SDLK_LEFT:
            snekDirection = SNEK_LEFT;
            break;
        case SDLK_RIGHT:
            snekDirection = SNEK_RIGHT;
            break;
    }
}
void populateFruit(int amount) {
    for (int i = 0; i < amount; i++) {
        appleMap[(rand()%mapSize.width) * mapSize.height + (rand()%mapSize.height)] = true;
    }
}
void gameLost() {
    memset(playerColisionMap, false, mapSize.width * mapSize.height * sizeof(bool));
    memset(appleMap, false, mapSize.width * mapSize.height * sizeof(bool));
    snekLength = 1;
    populateFruit(10);

}

void movePlayer() {
    memset(playerColisionMap, false, mapSize.width * mapSize.height * sizeof(bool));
    //push snek pieces, set collision map
    for (int i = snekLength - 1; i > 0; i--) {
        snek[i] = snek[i-1];
        playerColisionMap[(int)snek[i].x * mapSize.height + (int)snek[i].y] = true;
    }

    switch (snekDirection) {
        case SNEK_UP:
            snek[0].y -= 1;
            break;
        case SNEK_DOWN:
            snek[0].y += 1;
            break;
        case SNEK_LEFT:
            snek[0].x -= 1;
            break;
        case SNEK_RIGHT:
            snek[0].x += 1;
            break;
    }
    snek[0].direction = snekDirection;
    if (snek[0].x < 0) snek[0].x = mapSize.width - 1;
    if (snek[0].y < 0) snek[0].y = mapSize.height - 1;
    if (snek[0].x > mapSize.width - 1) snek[0].x = 0;
    if (snek[0].y > mapSize.height - 1) snek[0].y = 0;

    //check collision
    if (playerColisionMap[(int)snek[0].x * mapSize.height + (int)snek[0].y]) {
        gameLost();
    }
}

int WinMain() {

    uint64_t lastMoveTime = 0;

    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

    SDL_Window* wnd = SDL_CreateWindow("SNEK", 1280, 720, NULL);
    mapSize.width = 64;
    mapSize.height = 36;
    snek = malloc(sizeof(struct SnekPiece) * mapSize.width * mapSize.height);
    snek[0].x = 1.f;
    snek[0].y = 1.f;
    appleMap = malloc(mapSize.width * mapSize.height * sizeof(bool));
    playerColisionMap = malloc(mapSize.width * mapSize.height * sizeof(bool));
    memset(appleMap, false, mapSize.width * mapSize.height * sizeof(bool));

    //initialize rng
    srand(time(NULL));

    populateFruit(10);

    int snekDelay = 1000 / refreshRate;
    snekSpeed = 10.f / refreshRate;

    SDL_Renderer* renderer = SDL_CreateRenderer(wnd, NULL);
    SDL_SetRenderVSync(renderer, 1);
    //main loop
    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    isRunning = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    keyHandler(&event.key);
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetRenderScale(renderer, 20.0f, 20.0f);
        SDL_FRect rect = {0, 0, 1, 1};
        //render snek
        for (int i = 0; i < snekLength; i++) {
            rect.x = snek[i].x;
            rect.y = snek[i].y;
            SDL_RenderFillRect(renderer, &rect);
        }
        //render apple
        SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
        for (int i = 0; i <= mapSize.width; i++) {
            for (int j = 0; j <= mapSize.height; j++) {
                if (appleMap[i * mapSize.height + j]) {
                    rect.x = i;
                    rect.y = j;
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_RenderDebugTextFormat(renderer, 1, 1, "Snek v%.1f", version);
        SDL_RenderDebugTextFormat(renderer, 1, 11, "Length %d", snekLength);

        uint64_t currentTime = SDL_GetTicks();
        if (currentTime - 50 >= lastMoveTime) {
            movePlayer();
            if (appleMap[(int)snek[0].x * mapSize.height + (int)snek[0].y]) {
                snekLength++;
                snek[snekLength - 1] = snek[snekLength - 2];
                switch (snekDirection) {
                    case SNEK_UP:
                        snek[snekLength - 1].y -= 1.0;
                        break;
                    case SNEK_DOWN:
                        snek[snekLength - 1].y += 1.0;
                        break;
                    case SNEK_LEFT:
                        snek[snekLength - 1].x -= 1.0;
                        break;
                    case SNEK_RIGHT:
                        snek[snekLength - 1].x += 1.0;
                        break;
                }
                appleMap[(int)snek[0].x * mapSize.height + (int)snek[0].y] = false;
                appleMap[(rand()%mapSize.width) * mapSize.height + (rand()%mapSize.height)] = true;
            }
            lastMoveTime = currentTime;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}