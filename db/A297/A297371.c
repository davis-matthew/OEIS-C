/*
 * A297371  Number of nX5 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0, 1 or 2 neighboring 1s.
 * https://oeis.org/A297371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A297371
   Number of n×5 0..1 arrays with every 1 horizontally or antidiagonally adjacent to 0, 1 or 2 neighboring 1s.
   The code uses a dynamic‑programming automaton over rows, with a state that stores for each column
   whether the cell is present and how many more 1-neighbors it may receive from the next row
   (0,1,2).  There are 4 options per column (absent, rem‑0, rem‑1, rem‑2), i.e. 4⁵ = 1024 states. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>  /* for size_t */

/* Transition table: next_state[prev_state][curr_mask] = encoded state after adding curr_mask,
   or -1 if the transition is not allowed.  The table is built once the first time a() is called. */
#define STATE_SIZE 1024          /* 4⁵ */
static int16_t next_state[STATE_SIZE][32];
static bool table_built = false;

static void build_table(void)
{
    for (int prev = 0; prev < STATE_SIZE; ++prev) {
        /* decode prev state into 5 digits in base 4: s[j] = 0 absent, 1→rem‑0, 2→rem‑1, 3→rem‑2 */
        uint8_t s[5];
        int tmp = prev;
        for (int j = 0; j < 5; ++j) {
            s[j] = tmp & 3;
            tmp >>= 2;
        }
        for (int curr = 0; curr < 32; ++curr) {
            bool ok = true;

            /* 1. Check that a 1‑cell in the previous row can accept a below neighbour */
            for (int j = 0; j < 5 && ok; ++j) {
                if (s[j] > 0) {                     /* cell present in previous row   */
                    bool add = (j > 0) && ((curr >> (j - 1)) & 1);   /* neighbour below? */
                    if (add && (s[j] - 1) < 1)           /* remaining capacity too small */
                        ok = false;
                }
            }
            if (!ok) { next_state[prev][curr] = -1; continue; }

            /* 2. Compute the remaining allowed neighbours for the current row */
            uint8_t t[5] = {0, 0, 0, 0, 0};          /* encoded as 0 absent, 1→rem‑0 …, 3→rem‑2 */
            for (int j = 0; j < 5; ++j) {
                if ((curr >> j) & 1) {                /* current cell is 1 */
                    int horiz = 0;
                    if (j > 0 && ((curr >> (j - 1)) & 1)) ++horiz;
                    if (j < 4 && ((curr >> (j + 1)) & 1)) ++horiz;
                    int above = (j < 4 && s[j + 1] > 0) ? 1 : 0;        /* anti‑diagonal from previous row */
                    int sum = horiz + above;
                    if (sum > 2) { ok = false; break; }
                    int rem = 2 - sum;                 /* remaining allowed neighbours */
                    t[j] = rem + 1;                  /* encode 1…3 */
                } else {
                    t[j] = 0;                         /* cell absent */
                }
            }
            if (!ok) { next_state[prev][curr] = -1; continue; }

            /* 3. Encode the new state */
            int state = 0;
            for (int j = 4; j >= 0; --j) {
                state = (state << 2) | t[j];
            }
            next_state[prev][curr] = (int16_t)state;
        }
    }
    table_built = true;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!table_built) build_table();

    static seq_t dp_cur[STATE_SIZE], dp_next[STATE_SIZE];
    /* initialise for 0 rows (empty configuration) */
    for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;                          /* state 0 = all cells absent, no capacity left */

    for (idx_t row = 1; row <= n; ++row) {
        for (int i = 0; i < STATE_SIZE; ++i) dp_next[i] = 0;
        for (int prev = 0; prev < STATE_SIZE; ++prev) {
            seq_t val = dp_cur[prev];
            if (val == 0) continue;
            for (int curr = 0; curr < 32; ++curr) {
                int16_t ns = next_state[prev][curr];
                if (ns < 0) continue;
                dp_next[ns] += val;
            }
        }
        /* move next to current for the next iteration */
        for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all final states – the last row has no row below, so all are valid */
    seq_t total = 0;
    for (int i = 0; i < STATE_SIZE; ++i) total += dp_cur[i];
    return total;
}
