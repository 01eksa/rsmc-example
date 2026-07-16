#include "rsmc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CLEAR "\033[2J\033[H"

RsmcCoords ask_move(const RsmcMoves valid_moves)
{
    getchar(); // wait for Enter before making a random move
    return valid_moves.coords[rand() % valid_moves.count];
}

void show_game_state(const RsmcGameState state, const RsmcBoard *board)
{
    printf(CLEAR);

    for (int y = 0; y < RsmcBoardSize; y++) {
        for (int x = 0; x < RsmcBoardSize; x++) {
            switch (board->cells[y][x]) {
                case RsmcBoardCellBlack:
                    printf(" X ");
                    break;
                case RsmcBoardCellWhite:
                    printf(" O ");
                    break;
                default:
                    printf(" _ ");
                    break;
            }
        }
        printf("\n");
    }

    printf("Player 1 score: %d\n"
           "Player 2 score: %d\n",
           state.score.black_score, state.score.white_score);
}

void show_game_result(const RsmcGameStatus result)
{
    switch (result) {
        case RsmcGameStatusBlackWin:
            printf("X won!\n");
            break;
        case RsmcGameStatusWhiteWin:
            printf("O won!\n");
            break;
        case RsmcGameStatusDraw:
            printf("Draw\n");
            break;
        default:
            printf("No way that game didn't finish. Create an issue if you see this!\n");
            break;
    }
}

int main(void)
{
    srand(time(NULL));

    // first setup
    RsmcPlayer current_player = RsmcPlayerBlack;

    RsmcBoard board;
    rsmc_set_start_position(&board);
    RsmcGameState current_state;

    while (true) {
        // process moves
        const RsmcMoves valid_moves = rsmc_get_valid_moves(&board, current_player);

        if (valid_moves.count > 0) {
            const RsmcCoords chosen_move = ask_move(valid_moves);
            const bool ok = rsmc_apply_move(&board, chosen_move, current_player);
            assert(ok); // chosen_move is guaranteed valid, taken from valid_moves
        }

        // update info about game state
        current_state = rsmc_get_game_state(&board);
        show_game_state(current_state, &board);

        if (current_state.game_status != RsmcGameStatusContinue) {
            break; // game end
        }

        current_player = !current_player; // toggling guaranteed by API
    }

    show_game_result(current_state.game_status);

    return 0;
}
