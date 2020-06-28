// SDL2GameSnake.cpp : Defines the entry point for the application.
//

#include "SDL2GameSnake.h"

int my_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	std::cout << "Hello CMake." << std::endl;

    return my_main(argc, argv);

	return 0;
}

#include <time.h>

#define W 480   
#define H 800   
#define Max 100   

struct Food
{
    int x;
    int y;
    int yes;
} food;                         // 食物    
struct Game
{
    int score;                  // score   
    int level;
    int speed;
} game;                         // 游戏分数 级别 速度   
struct Snake
{
    int x[Max];
    int y[Max];
    int yes;                    // 生命   
    int dir;
    int node;
} snake;

typedef char string[50];


SDL_Rect Rfood;                 // 食物大小及坐标 // 食物的宽高   
SDL_Rect joy;                   // 摇杆大小 坐标   
SDL_Rect Rsnake;                // 蛇大小 坐标   
SDL_Rect Rblock;
SDL_Rect Rword;                 // 话语框   
SDL_Rect Rprint;                // 用于分数 关卡 级别的输出   
SDL_Event event;                // 事件   

const string str_head_1 = "res/head1.bmp";
const string str_head_2 = "res/head2.bmp";
const string str_head_3 = "res/head3.bmp";
const string str_head_4 = "res/head4.bmp";

const string str_body = "res/body.bmp";
const string str_food = "res/food.png";
const string str_back1 = "res/back1.bmp";
const string str_back2 = "res/back2.bmp";
const string str_back3 = "res/back3.bmp";
const string str_back4 = "res/back4.bmp";
const string str_block = "res/block.bmp";

// 加载图片   
SDL_Texture* Tmainpicture = 0;
SDL_Texture* Tsnake_head = 0;   // 蛇头   
SDL_Texture* Tsnake_body = 0;   // 蛇身   
SDL_Texture* Tbackground = 0;   // 背景   
SDL_Texture* Tfood = 0;         // 食物   
SDL_Texture* Tblock = 0;        // 边界   

void GameInit(void);            // 游戏初始化   
struct Food GetFood(struct Food food);  // 获得食物   
void JudgeKey(void);            // 判断按键   
void SnakeDraw();           // 画出蛇   
void ChangeHead();          // 判断蛇头方向   
void SnakeMove(void);           // 蛇移动   
void IfEat(void);               // 蛇是否与边界，食物相碰   
void SnakeBump(void);           // 判断蛇是否出界   
void BlockDraw(void);           // 画出边届界   
void Textprint(void);           // 文本输出   
void NextInit(void);
bool MainChoose();

SDL_Window* win = NULL;         // 窗口   
SDL_Renderer* ren = NULL;
SDL_Color color = { 238, 18, 137 };
SDL_Color Cword = { 200, 40, 150 };

char cha[40];
bool bool_level = true;         // 图片更新   

TTF_Font* Fprint = 0;           // 分数 级别输出   
TTF_Font* Fword = 0;            // 话语框   

Mix_Music *Mback = 0;         // 背景音乐   

Mix_Music* Mover = 0;

Mix_Music* Meat = 0;

Uint32 tickStart = 0;

int my_main(int argc, char* argv[])
{
    GameInit();

    if (!MainChoose())
        exit(0);

    bool GamePlay = true;

    while (GamePlay)
    {
        if (SDL_PollEvent(&event))
        {
            JudgeKey();
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    GamePlay = 0;
                }
            }
            if (event.type == SDL_QUIT)
            {
                GamePlay = 0;
                printf("\n\nquit!\n\n*******\n");
            }
        }
        else
        {
            if (GamePlay && (tickStart == 0 || SDL_GetTicks() - tickStart >= game.speed))
            {
                SDL_RenderClear(ren);
                SDL_RenderCopy(ren, Tbackground, 0, 0);
                BlockDraw();
                if (food.yes == 0)
                    food = GetFood(food);
                food.yes = 1;
                if (food.yes == 1)  // 显示食物   
                {
                    Rfood.x = food.x;
                    Rfood.y = food.y;
                    SDL_RenderCopy(ren, Tfood, 0, &Rfood);
                }
                IfEat();            // 判断语句   
                SnakeMove();        // 蛇移动   
                ChangeHead();       // 改变蛇头方向   
                SnakeDraw();        // 画出蛇   
                SnakeBump();        // 判断蛇是否出界   
                Textprint();
                NextInit();         // 对下一关，初始化   
                SDL_RenderPresent(ren);

                //SDL_Delay(game.speed);
                if (snake.yes == 0)
                {
                    GamePlay = false;
                }
                tickStart = SDL_GetTicks();
            }
        }  
    }
    Mix_FreeMusic(Mback);   

    Mix_FreeMusic(Meat);
    Mix_FreeMusic(Mover);
    Mix_CloseAudio();
    SDL_DestroyTexture(Tbackground);
    SDL_DestroyTexture(Tsnake_head);
    SDL_DestroyTexture(Tsnake_body);
    SDL_DestroyTexture(Tfood);
    SDL_DestroyTexture(Tblock);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    Mix_Quit();
    SDL_Quit();
    return 0;
}

bool MainChoose()
{
    // 选择模式   
    bool choose = true; 
    SDL_Event e;
    SDL_Surface* sur1 = 0, * sur2 = 0;
    SDL_Texture* tex1 = 0, * tex2 = 0;
    SDL_Rect rec;
    float x = 0, y = 0;
    rec.x = W / 2 - 50;
    rec.y = H / 2 + 100;
    rec.w = 100;
    rec.h = 80;

    sur1 = TTF_RenderUTF8_Blended(Fword, "Start", Cword);
    tex1 = SDL_CreateTextureFromSurface(ren, sur1);

    sur2 = TTF_RenderUTF8_Blended(Fword, "Exit", Cword);
    tex2 = SDL_CreateTextureFromSurface(ren, sur2);

    SDL_FreeSurface(sur1);  
    SDL_FreeSurface(sur2);

    while (choose)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    x = e.button.x;
                    y = e.button.y;
                    if (y >= H / 2 + 100 && y <= H / 2 + 220)
                    {
                        printf("Start Game\n");
                        choose = false;
                        return true;
                    }
                    else if (y > H / 2 + 220 && y <= H / 2 + 300)
                    {
                        printf("Exit Game\n");
                        return false;
                    }
                }
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    printf("Start Game\n");
                    choose = false;
                    return true;
                }
            }
            if (e.type == SDL_QUIT)
            {
                printf("Exit Game\n");
                return false;
            }
        }
        else
        {
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, Tmainpicture, 0, 0);
            SDL_RenderCopy(ren, tex1, 0, &rec);
            rec.y += 120;
            SDL_RenderCopy(ren, tex2, 0, &rec);
            rec.y = H / 2 + 100;

            SDL_RenderPresent(ren);
        }
    }
}

struct Food GetFood(struct Food food)
{                               
    // 创建符合的食物   
    bool exist = false;
    int j;  
    while (!exist)
    {
        exist = true;  
        food.x = rand() % 390 + 30;
        while (food.x % 30)
            food.x++;
        food.y = rand() % 540 + 30; // 生成符合的食物    
        while (food.y % 30)
            food.y++;
        for (j = 0; j < snake.node; j++)
            if (food.x == snake.x[j] && food.y == snake.y[j])
                exist = false;
    }
    return food;
}

void GameInit(void)
{                               // 初始化   
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        printf("SDL_INIT fault!\n");
        exit(0);
    }
    win = SDL_CreateWindow("SNAKE!", 100, 100, W, H, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(ren, 480, 800);

    srand((unsigned)time(0));   //

    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();
    Mback = Mix_LoadMUS("res/back.mp3");   

    Meat = Mix_LoadMUS("res/eat.mp3");
    Mover = Mix_LoadMUS("res/gameover.mp3");
    Fprint = TTF_OpenFont("res/Roguee.ttf", 30);
    Fword = TTF_OpenFont("res/Roguee.ttf", 50);

    game.speed = 300;
    game.score = 0;
    game.level = 1;
    snake.node = 2;
    snake.yes = 1;
    snake.dir = 2;
    food.yes = 0;

    Tmainpicture = IMG_LoadTexture(ren, "res/background.png");
    Tsnake_head = IMG_LoadTexture(ren, str_head_2);
    Tsnake_body = IMG_LoadTexture(ren, str_body);
    Tbackground = IMG_LoadTexture(ren, str_back1);
    Tfood = IMG_LoadTexture(ren, str_food);

    Tblock = IMG_LoadTexture(ren, str_block);
    SDL_QueryTexture(Tfood, 0, 0, &Rfood.w, &Rfood.h);

    SDL_QueryTexture(Tsnake_head, 0, 0, &Rsnake.w, &Rsnake.h);
    SDL_QueryTexture(Tblock, 0, 0, &Rblock.w, &Rblock.h);
    joy.x = 280;
    joy.y = 610;
    snake.x[0] = 270;
    snake.y[0] = 150;
    snake.x[1] = 270;
    snake.y[1] = snake.y[0] - Rsnake.h;
    Rword.x = W / 2 - 200;
    Rword.y = H / 2 - 200;
    Rword.w = 400;
    Rword.h = 200;
    Rprint.x = 30;
    Rprint.y = 30;
    Rprint.w = 150;
    Rprint.h = 30;
}

void JudgeKey(void)
{                              
    // 判断按键   
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_UP && snake.dir != 2) snake.dir = 1;
        else if (event.key.keysym.sym == SDLK_DOWN && snake.dir != 1) snake.dir = 2;
        else if (event.key.keysym.sym == SDLK_LEFT && snake.dir != 4) snake.dir = 3;
        else if (event.key.keysym.sym == SDLK_RIGHT && snake.dir != 3)snake.dir = 4;
    }
}

void SnakeDraw()
{
    int i;
    for (i = 0; i < snake.node; i++)
    {
        Rsnake.x = snake.x[i];
        Rsnake.y = snake.y[i];
        if (i == 0)
            SDL_RenderCopy(ren, Tsnake_head, 0, &Rsnake);
        else
            SDL_RenderCopy(ren, Tsnake_body, 0, &Rsnake);
    }
}

void BlockDraw(void)
{
    void GamePrint(string word, TTF_Font * fon, SDL_Color color, SDL_Rect rect,
        bool change);
    int i;
    SDL_Texture* tex;
    SDL_Surface* sur;
    Rblock.x = 0;
    Rblock.y = 0;
    for (i = 0; i < W; i += Rblock.w)
    {
        Rblock.x = i;
        SDL_RenderCopy(ren, Tblock, 0, &Rblock);
        Rblock.y = H - Rblock.h;
        SDL_RenderCopy(ren, Tblock, 0, &Rblock);
        Rblock.y = 0;
    }

    Rblock.x = 0;
    for (i = 0; i < H; i += Rblock.h)
    {
        Rblock.y = i;
        SDL_RenderCopy(ren, Tblock, 0, &Rblock);
        Rblock.x = W - Rblock.w;
        SDL_RenderCopy(ren, Tblock, 0, &Rblock);
        Rblock.x = 0;
    }

    sprintf(cha, "Score %d    Level %d", game.score, game.level);    // 把坐标位置储存到ch字符串中,这里乘以100为了显示更方便直观   
    sur = TTF_RenderUTF8_Blended(Fprint, cha, color);   // 让字符串生成surface表面并且传递给sur2   
    tex = SDL_CreateTextureFromSurface(ren, sur);
    SDL_FreeSurface(sur);
    SDL_RenderCopy(ren, tex, NULL, &Rprint);
    SDL_DestroyTexture(tex);
}

void ChangeHead()
{
    SDL_DestroyTexture(Tsnake_head);
    if (snake.dir == 1) Tsnake_head = IMG_LoadTexture(ren, str_head_1);
    if (snake.dir == 2) Tsnake_head = IMG_LoadTexture(ren, str_head_2);
    if (snake.dir == 3) Tsnake_head = IMG_LoadTexture(ren, str_head_3);
    if (snake.dir == 4) Tsnake_head = IMG_LoadTexture(ren, str_head_4);
}

void SnakeMove(void)
{
    int i;
    for (i = snake.node - 1; i > 0; i--)
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }
    switch (snake.dir)
    {                           // 蛇在无响应时自动移动   
    case 1:
        snake.y[0] -= 30;
        break;
    case 2:
        snake.y[0] += 30;
        break;
    case 3:
        snake.x[0] -= 30;
        break;
    case 4:
        snake.x[0] += 30;
        break;
    }
}

void IfEat(void)
{                               // 判断是否吃食物   
    int ix, iy;
    bool change = false;
    if (snake.dir == 4)
    {
        ix = snake.x[0] + 30;
        iy = snake.y[0];
        if (ix == food.x && iy == food.y)
            change = true;
    }
    else if (snake.dir == 3)
    {
        ix = food.x + 30;
        iy = food.y;
        if (ix == snake.x[0] && iy == snake.y[0])
            change = true;
    }
    else if (snake.dir == 1)
    {
        ix = food.x;
        iy = food.y + 30;
        if (ix == snake.x[0] && iy == snake.y[0])
            change = true;
    }
    else if (snake.dir == 2)
    {
        ix = snake.x[0];
        iy = snake.y[0] + 30;
        if (ix == food.x && iy == food.y)
            change = true;
    }
    if (change == true)
    {
        Mix_PlayMusic(Meat, 0);
        food.yes = 0;
        snake.node++;
        game.score += 100;
    }
}
void Textprint(void)
{
    // 判断并输出   
    SDL_Surface* sur = 0;
    SDL_Texture* tex = 0;
    if (game.score == 1000 || game.score == 2500 || game.score == 4000
        || game.score == 6000)
    {
        game.level++;
        game.score += 100;
        bool_level = true;
        sur = TTF_RenderUTF8_Blended(Fword, "Next Level", Cword);   // 让字符串生成surface表面并且传递给sur   

    }
    else if (snake.yes == 0)
    {
        sur = TTF_RenderUTF8_Blended(Fword, "Game over", Cword);    // 让字符串生成surface表面并且传递给sur   
        Mix_PauseMusic();   
        Mix_PlayMusic(Mover, 0);
        SDL_Delay(2000);
    }
    tex = SDL_CreateTextureFromSurface(ren, sur);
    if(sur != NULL) SDL_FreeSurface(sur);
    if (tex != NULL)
    {
        SDL_RenderCopy(ren, tex, NULL, &Rword);
        SDL_DestroyTexture(tex);
    }
}

void SnakeBump(void)
{
    // 判断蛇是否出界   
    int j;
    if (snake.x[0] <= 0 || snake.x[0] >= W - Rblock.w || snake.y[0] <= 0 || snake.y[0] >= H - Rblock.h)
        snake.yes = 0;
    else
    {//判断蛇头是否咬到自己   
        for (j = 4; j < snake.node; j++)
            if (snake.x[0] == snake.x[j] && snake.y[0] == snake.y[j])
                snake.yes = 0;
    }
}

void NextInit(void)
{
    // 游戏背景的选择和对下一关的初始化   
    if (bool_level == true)
    {
        SDL_DestroyTexture(Tbackground);
        if (game.level == 1)
        {
            Tbackground = IMG_LoadTexture(ren, str_back1);
            game.speed = 300;
        }
        else if (game.level == 2)
        {
            Tbackground = IMG_LoadTexture(ren, str_back2);
            game.speed = 200;
        }
        else if (game.level == 3)
        {
            Tbackground = IMG_LoadTexture(ren, str_back3);
            game.speed = 100;
        }
        else if (game.level == 4)
        {
            Tbackground = IMG_LoadTexture(ren, str_back4);
            game.speed = 50;
        }
        else
        {
            game.level = 1;
            game.speed = 300;
        }


        snake.x[0] = 270;
        snake.y[0] = 150;
        snake.x[1] = 240;
        snake.y[1] = 150;
        snake.node = 2;
        snake.dir = 2;
        bool_level = false;
        Mix_PlayMusic(Mback, -1);   
    }
}

