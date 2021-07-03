#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
    int err = SDL_Init(SDL_INIT_VIDEO);

    if(err > 0)
    {
        SDL_Log("Can't init %s", SDL_GetError());
        return err;
    }

    SDL_Window *window = SDL_CreateWindow("Click Ball", 100, 100, 800, 600, 0);
    if(window == nullptr)
    {
        SDL_Log("SDL_CreateWindow failed. Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr)
    {
        SDL_Log("SDL_CreateRenderer failed. Error: %s", SDL_GetError());
        return 1;
    }

    // SDL_LoadBMP carrega uma superfície, que é uma imagem na memória de CPU.
    SDL_Surface *surface = SDL_LoadBMP("ball.bmp");

    // SDL_SetColorKey serve para indicar a cor transparente da imagem.
    SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 255, 0, 255));

    // SDL_CreateTextureFromSurface manda a imagem para a memória de vídeo, para um desempenho melhor.
    SDL_Texture *ballTexture = SDL_CreateTextureFromSurface(renderer, surface);

    // SDL_FreeSurface libera a memória de CPU.
    SDL_FreeSurface(surface);

    struct Ball
    {
        SDL_Rect box = { 0, 0, 100, 100 };
        SDL_Point velocity = { 1, 1 };
    };

    Ball ball;

    bool running = true;

    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                SDL_Point mousePosition = { event.button.x, event.button.y };
                //  SDL_EnclosePoints pega uma array de pontos e retorna verdadeiro no caso de todos os pontos estiverem dentro do retângulo informado.
                bool ballClicked = SDL_EnclosePoints(&mousePosition, 1, &ball.box, nullptr);
                if(event.button.button == SDL_BUTTON_LEFT && ballClicked)
                {
                    Sint16 absVelocity = abs(ball.velocity.x);
                    if(absVelocity < 32)
                    {
                        ball.velocity.y = ball.velocity.x = absVelocity + 1;
                        if(rand() % 2 == 0)
                        {
                            ball.velocity.x = -ball.velocity.x;
                        }
                        if(rand() % 2 == 0)
                        {
                            ball.velocity.y = -ball.velocity.y;
                        }
                    }
                }

                break;
            }
            
            default:
                break;
            }
        }

        ball.box.x += ball.velocity.x;
        ball.box.y += ball.velocity.y;

        if(ball.box.x < 0 || ball.box.x + ball.box.w >= 800)
        {
            ball.velocity.x = -ball.velocity.x;
        }
        if(ball.box.y < 0 || ball.box.y + ball.box.h >= 600)
        {
            ball.velocity.y = -ball.velocity.y;
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 127, 0, 127, 255);
        // SDL_RenderFillRect(renderer, &ball.box);
        SDL_RenderCopy(renderer, ballTexture, nullptr, &ball.box);

        SDL_RenderPresent(renderer);


        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}