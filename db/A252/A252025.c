/*
 * A252025  Number of (n+2)X(1+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 2 or 4.
 * https://oeis.org/A252025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP on the last two rows of a (n+2)x3 array over {0,1,2} such
 *            that every 3x3 sub‑block has all row, column, main diagonal, and
 *            anti‑diagonal sums different from 0, 2 and 4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 47, u128 n <= 99,
 *            big any n
 * Helpers:   block_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/block_valid.h"

/* 3³ possible rows, 27*27 states for two consecutive rows */
enum { ROWS   = 27,            /* number of distinct 3‑digit rows */
       STATES = ROWS * ROWS }; /* pair of consecutive rows */

/* Decode a base‑3 code into a 3‑digit row. */
static inline void decode_row(int idx, uint8_t r[3]) {
    r[0] = idx % 3;
    r[1] = (idx / 3) % 3;
    r[2] = (idx / 9) % 3;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* sequence is defined for n >= 1 */

    /* Transition tables (built once). */
    static bool initialized = false;
    static uint8_t rows[ROWS][3];
    static unsigned short next_count[STATES];
    static int next_list[STATES][ROWS]; /* max ROWS successors per state */

    if (!initialized) {
        /* Enumerate all rows. */
        for (int i = 0; i < ROWS; ++i)
            decode_row(i, rows[i]);

        /* Initialise counts. */
        for (int s = 0; s < STATES; ++s)
            next_count[s] = 0;

        /* Build the transition graph. */
        for (int r1 = 0; r1 < ROWS; ++r1)
            for (int r2 = 0; r2 < ROWS; ++r2)
                for (int r3 = 0; r3 < ROWS; ++r3)
                    if (block_valid(rows[r1], rows[r2], rows[r3])) {
                        int old_state = r1 * ROWS + r2;
                        int new_state = r2 * ROWS + r3;
                        unsigned short idx = next_count[old_state]++;
                        next_list[old_state][idx] = new_state;
                    }
        initialized = true;
    }

    /* DP tables: two rows for current and next counts. */
    static seq_t dp[2][STATES];
    int cur = 0, nxt = 1;

    /* Initial distribution: first two rows are arbitrary. */
    for (int s = 0; s < STATES; ++s)
        dp[cur][s] = 1;

    /* Perform n transitions (each adds one more row). */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero next vector. */
        for (int s = 0; s < STATES; ++s)
            dp[nxt][s] = 0;

        /* Transition from current to next. */
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp[cur][s];
            if (!val) continue;
            unsigned short cnt = next_count[s];
            for (unsigned short k = 0; k < cnt; ++k) {
                int ns = next_list[s][k];
                dp[nxt][ns] += val;
            }
        }

        /* Advance the pointers. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all final states to get total number of arrays. */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s)
        result += dp[cur][s];

    return result;
}
