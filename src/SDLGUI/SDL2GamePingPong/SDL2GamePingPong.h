// sdlgui.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifdef main
#undef main
#endif // main

// TODO: Reference additional headers your program requires here.
// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2
{
    float x;
    float y;
};

// Game class
class Game
{
public:
    Game();
    // Initialize the game
    bool Initialize();
    // Runs the game loop until the game is over
    void RunLoop();
    // Shutdown the game
    void Shutdown();
private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Window created by SDL
    SDL_Window* mWindow;
    // Renderer for 2D drawing
    SDL_Renderer* mRenderer;
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;

    // Pong specific
    // Direction of paddle
    int mPaddleDir;
    // Position of paddle
    Vector2 mPaddlePos;
    // Position of ball
    Vector2 mBallPos;
    // Velocity of ball
    Vector2 mBallVel;
};
