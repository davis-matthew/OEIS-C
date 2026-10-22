/*
 * A390650  Number of tic-tac-toe positions after n plays.
 * https://oeis.org/A390650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Enumerates all legal tic‑tac‑toe positions up to 9 moves (after
 *            which the board is full). For n > 9 the function returns 0
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Maximum number of distinct board states: 3^9 = 19683 */
#define MAX_STATES 19683
#define MAX_DEPTH 10

static const int pow3[9] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561};

/* Precomputed winner table for every board state */
static bool win[MAX_STATES] = {0};

static bool init_done = false;

static void init_win_table(void)
{
    for (int s = 0; s < MAX_STATES; ++s) {
        int tmp = s;
        int cell[9];
        for (int i = 0; i < 9; ++i) {
            cell[i] = tmp % 3;
            tmp /= 3;
        }
        bool w = false;
        /* 8 winning lines */
        const int lines[8][3] = {
            {0,1,2},{3,4,5},{6,7,8},
            {0,3,6},{1,4,7},{2,5,8},
            {0,4,8},{2,4,6}
        };
        for (int l = 0; !w && l < 8; ++l) {
            int a = lines[l][0], b = lines[l][1], c = lines[l][2];
            if (cell[a] && cell[a] == cell[b] && cell[a] == cell[c])
                w = true;
        }
        win[s] = w;
    }
}

seq_t a(idx_t n)
{
    static seq_t counts[MAX_DEPTH] = {0};

    if (!init_done) {
        init_win_table();
        /* BFS levels of depth 0..9 */
        static uint16_t cur[MAX_STATES];
        static uint16_t next[MAX_STATES];
        static bool visited_next[MAX_STATES];
        int cur_len = 1, next_len = 0;
        cur[0] = 0; /* empty board */

        for (int depth = 0; depth < MAX_DEPTH; ++depth) {
            counts[depth] = (seq_t)cur_len;
            if (cur_len == 0) break;

            int player = (depth % 2 == 0) ? 1 : 2; /* 1 = X, 2 = O */

            /* clear visited array for the next depth */
            for (int i = 0; i < MAX_STATES; ++i)
                visited_next[i] = false;

            next_len = 0;
            for (int i = 0; i < cur_len; ++i) {
                uint16_t state = cur[i];
                if (win[state]) continue; /* terminal, no children */

                for (int p = 0; p < 9; ++p) {
                    int cell = (state / pow3[p]) % 3;
                    if (cell == 0) {
                        uint16_t new_state = state + player * pow3[p];
                        if (!visited_next[new_state]) {
                            visited_next[new_state] = true;
                            next[next_len++] = new_state;
                        }
                    }
                }
            }

            /* prepare for next iteration */
            for (int i = 0; i < next_len; ++i)
                cur[i] = next[i];
            cur_len = next_len;
        }
        init_done = true;
    }

    if (n < MAX_DEPTH)
        return counts[(idx_t)n];
    else
        return (seq_t)0;
}
