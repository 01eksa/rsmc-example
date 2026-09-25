#include "rsmc.h"

#include <stdbool.h>
#include <stdio.h>

#define CLEAR "\033[2J\033[H"

// new ABI is optimized for efficiency and does not have much abstractions
// so I'll use this helper to convert loop indexes into bitmasks in one line
static RsmcBitMask bitmask_from_x_y(const int x, const int y)
{
    const int index = y * RsmcBoardSize + x;
    return rsmc_index_to_bitmask(index);
}

static RsmcBitMask ask_move(const RsmcBitMap valid_moves, const RsmcPlayer player)
{
    while (true) {
        // get coords from user
        char x, y;
        printf("Enter moves as two digits (e.g. 3 4). Board is 0-indexed.\n");
        printf("%s: ", player == RsmcPlayerBlack ? "X" : "O");
        scanf(" %c %c", &x, &y);

        x -= '0';
        y -= '0';

        // convert coords into bitmask
        const RsmcBitMask move = bitmask_from_x_y(x, y);

        // simple check
        if (valid_moves & move) {
            return move;
        }

        printf("Invalid move!\n");
    }
}

static void show_game_state(const RsmcGameState state, const RsmcBoard board,
                            const RsmcBitMask valid_moves)
{
    printf(CLEAR);


    printf("X score: %d\n"
           "O score: %d\n",
           state.score.black_score, state.score.white_score);
    for (int y = 0; y < RsmcBoardSize; y++) {
        for (int x = 0; x < RsmcBoardSize; x++) {
            const RsmcBitMask cell_mask = bitmask_from_x_y(x, y);

            if (cell_mask & board.black) {
                printf(" X ");
            } else if (cell_mask & board.white) {
                printf(" O ");
            } else if (cell_mask & valid_moves) {
                printf(" * ");
            } else {
                printf(" _ ");
            }
        }
        printf("\n");
    }
}

static void show_game_result(const RsmcGameStatus result)
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
    // first setup
    RsmcPlayer current_player = RsmcPlayerBlack;
    RsmcBoard board = rsmc_get_start_position();
    RsmcGameState current_state = rsmc_get_game_state(board);

    do {
        // process moves
        const RsmcBitMap valid_moves = rsmc_get_valid_moves(board, current_player);
        show_game_state(current_state, board, valid_moves);

        if (valid_moves) {
            const RsmcBitMask chosen_move = ask_move(valid_moves, current_player);
            board = rsmc_apply_move(board, chosen_move, current_player);
        }

        // update info about game state
        current_player = !current_player; // toggling guaranteed by API
        current_state = rsmc_get_game_state(board);
    } while (current_state.game_status == RsmcGameStatusContinue);

    show_game_result(current_state.game_status);
    return 0;
}
