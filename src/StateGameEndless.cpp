#include "StateGameEndless.h"

#include "Game.h"

#include <map>
#include <string>

StateGameEndless::StateGameEndless(Game * p) : State(p)
{
    lDEBUG << Log::CON("StateGame");

    setState(eInitial);

    // Initialise game indicator
    mGameIndicators.setGame(p, this);

    // Initialise game board
    mGameBoard.setGame(p, this);

    // Load the loading screen
    mImgLoadingBanner.setWindowAndPath(p, "media/loadingBanner.png");
}

StateGameEndless::~StateGameEndless ()
{
    lDEBUG << Log::DES("StateGame");
}

void StateGameEndless::update()
{
    // On the eInitial state, don't do anything about logic
    if (mState == eInitial)
    {
        return;
    }

    // On this state, start loading the resources
    else if (mState == eStartLoading)
    {
        loadResources();
        setState(eSteady);

        // Start the clock
        resetTime();

        // Reset the scoreboard
        mGameIndicators.setScore(0);
    }

    // Compute remaining time
    double elapsedTime = (mTimeStart + SDL_GetTicks()) / 1000;

    mGameIndicators.updateTime(elapsedTime);

    if (remainingTime <= 0)
    {
        // Tell the board that the game ended with the given score
        mGameBoard.endGame(mGameIndicators.getScore());
    }

    mGameBoard.update();
}

void StateGameEndless::draw()
{
    // On this state, show the loading screen and switch the state
    if (mState == eInitial)
    {
        mImgLoadingBanner.draw(156, 200, 2);
        setState(eStartLoading);

        return;
    }

    // In all the other states, the full window is drawn
    mImgBoard.draw(0,0,0);

    // Draw the indicators (buttons and labels)
    mGameIndicators.draw();

    // Draw the main game board
    mGameBoard.draw();
}

void StateGameEndless::buttonDown(SDL_Keycode button)
{
    if (button == SDLK_ESCAPE)
    {
        mGame -> changeState("stateMainMenu");
    }

    else if (button == SDLK_h)
    {
        showHint();
    }
}

void StateGameEndless::mouseButtonDown(Uint8 button)
{
    // Left mouse button was pressed
    if (button == SDL_BUTTON_LEFT)
    {
        mMousePressed = true;

        // Get click location
        int mouseX = mGame->getMouseX();
        int mouseY = mGame->getMouseY();

        // Inform the UI
        mGameIndicators.click(mouseX, mouseY);

        // Inform the board
        mGameBoard.mouseButtonDown(mouseX, mouseY);
    }
}

void StateGameEndless::mouseButtonUp(Uint8 button)
{
    // Left mouse button was released
    if (button == SDL_BUTTON_LEFT)
    {
        mMousePressed = false;

        // Get click location
        int mouseX = mGame->getMouseX();
        int mouseY = mGame->getMouseY();

        // Inform the board
        mGameBoard.mouseButtonUp(mouseX, mouseY);
    }
}

void StateGameEndless::setState (tState state)
{
    // static std::map<tState, std::string> stateToString = {
    //     { eInitial, "eInitial"},
    //     { eStartLoading, "eStartLoading"},
    //     { eLoading, "eLoading"},
    //     { eLaunched, "eLaunched"},
    //     { eNewGemsFalling, "eNewGemsFalling"},
    //     { eOldGemsFalling, "eOldGemsFalling"},
    //     { eWaiting, "eWaiting"},
    //     { eGemSelected, "eGemSelected"},
    //     { eGemsSwitching, "eGemsSwitching"},
    //     { eGemsDisappearing, "eGemsDisappearing"},
    //     { eFirstFlip, "eFirstFlip"},
    //     { eInicialGemas, "eInicialGemas"},
    //     { eEspera, "eEspera"},
    //     { eGemaMarcada, "eGemaMarcada"},
    //     { eGemasCambiando, "eGemasCambiando"},
    //     { eGemasDesapareciendo, "eGemasDesapareciendo"},
    //     { eGemasNuevasCayendo, "eGemasNuevasCayendo"},
    //     { eDesapareceBoard, "eDesapareceBoard"},
    //     { eTimeFinished, "eTimeFinished"},
    //     { eShowingScoreTable, "eShowingScoreTable"}
    // };

    // lDEBUG << "New state: " << stateToString[state];
    mState = state;
}

// ----------------------------------------------------------------------------

void StateGameEndless::loadResources()
{
    // Load the background image
    mImgBoard.setWindowAndPath(mGame, "media/board.png");

    mGameIndicators.loadResources();
    mGameBoard.loadResources();
}

void StateGameEndless::resetGame()
{
    resetTime();
    mGameBoard.resetGame();
}

void StateGameEndless::resetTime()
{
    // Default time is 2 minutes
    mTimeStart = SDL_GetTicks() + 2 * 60 * 1000;
}

void StateGameEndless::showHint()
{
    mGameBoard.showHint();
}

void StateGameEndless::increaseScore (int amount)
{
    mGameIndicators.increaseScore(amount);
}

