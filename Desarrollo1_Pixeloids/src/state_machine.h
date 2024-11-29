#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

namespace pixeloids_luchelli
{

    enum class GameStates {
        MENU,
        HOW_TO_PLAY,
        PLAYING,
        PAUSED,
        GAME_OVER,
        CREDITS,
        EXIT
    };
    struct GameStateMachine
    {
        GameStates currentState;
        GameStates nextState;
    };


} // namespace pixeloids_luchelli

#endif // !STATE_MACHINE_H

