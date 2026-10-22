/*
 * A300172  Number of nX5 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the number of n×5 0/1 arrays with the king‑move
   adjacency rule (each cell must have 2–8 equal neighbours), and the
   upper‑left element fixed to 0.  The rule depends on three consecutive
   rows, so we use a dynamic program with state (prev‑prev row, prev row)
   and pre‑compute a transition table that tells, for a given pair
   (p2,p1), which rows p0 can follow.  All arithmetic is done in
   seq_t.  Complexity: O(n·32³) ≈ 3·10⁵ operations for n=10 000. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: check that a given row is valid with a possible neighbour above
   (p2) and below (p1).  has_up/has_low flag indicate whether those rows
   exist (outside of the board).  The row bits are: bit 0 → column 0. */
static bool valid_row(uint8_t upper, uint8_t cur, uint8_t lower,
                      bool has_up, bool has_low)
{
    for (int c = 0; c < 5; ++c) {
        int val = (cur >> c) & 1;
        int cnt = 0;

        /* same‑row neighbours */
        if (c > 0 && ((cur >> (c - 1)) & 1) == val) ++cnt;
        if (c < 4 && ((cur >> (c + 1)) & 1) == val) ++cnt;

        /* upper‑row neighbours */
        if (has_up) {
            if (c > 0 && ((upper >> (c - 1)) & 1) == val) ++cnt;
            if (((upper >> c) & 1) == val) ++cnt;
            if (c < 4 && ((upper >> (c + 1)) & 1) == val) ++cnt;
        }

        /* lower‑row neighbours */
        if (has_low) {
            if (c > 0 && ((lower >> (c - 1)) & 1) == val) ++cnt;
            if (((lower >> c) & 1) == val) ++cnt;
            if (c < 4 && ((lower >> (c + 1)) & 1) == val) ++cnt;
        }

        if (cnt < 2 || cnt > 8) return false;
    }
    return true;
}

/* Transition table: transitions[p2][p1] gives a 32‑bit mask of all rows
   that can follow the pair (p2,p1). */
static uint32_t transitions[32][32];

/* Initialise transition table once. */
static void init_transitions(void)
{
    for (int p2 = 0; p2 < 32; ++p2)
        for (int p1 = 0; p1 < 32; ++p1) {
            uint32_t mask = 0;
            for (int cur = 0; cur < 32; ++cur)
                if (valid_row((uint8_t)p2, (uint8_t)p1,
                              (uint8_t)cur, true, true))
                    mask |= (1u << cur);
            transitions[p2][p1] = mask;
        }
}

/* Compute a(n).  n is 1‑based index; a(1)=0. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* a(1)=0 and any n<2 has no valid arrays */

    static bool init_done = false;
    if (!init_done) {
        init_transitions();
        init_done = true;
    }

    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* Initialise all DP entries to zero. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* First two rows: top row must have its leftmost bit (column 0) = 0. */
    for (int r0 = 0; r0 < 32; ++r0) {
        if (r0 & 1) continue;           /* enforce top‑left zero */
        for (int r1 = 0; r1 < 32; ++r1)
            if (valid_row(0, (uint8_t)r0, (uint8_t)r1, false, true))
                dp[r0][r1] = 1;
    }

    /* Process remaining rows: rows 2 … n‑1 (total n‑2 rows). */
    for (idx_t step = 0; step < n - 2; ++step) {
        /* Zero ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int p2 = 0; p2 < 32; ++p2)
            for (int p1 = 0; p1 < 32; ++p1) {
                seq_t cnt = dp[p2][p1];
                if (cnt == 0) continue;
                uint32_t mask = transitions[p2][p1];
                while (mask) {
                    int cur = __builtin_ctz(mask);
                    mask &= mask - 1;
                    ndp[p1][cur] += cnt;
                }
            }

        /* Swap dp and ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final row (row n‑1) has no lower neighbour. Sum up all valid states. */
    seq_t ans = 0;
    for (int p2 = 0; p2 < 32; ++p2)
        for (int p1 = 0; p1 < 32; ++p1) {
            seq_t cnt = dp[p2][p1];
            if (cnt == 0) continue;
            if (valid_row((uint8_t)p2, (uint8_t)p1, 0, true, false))
                ans += cnt;
        }

    return ans;
}
