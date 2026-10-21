/*
 * A263055  Number of (n+1)X(3+1) 0..1 arrays with each row and column not
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A263055 using DP over rows with column values modulo 3.  
   Ref.: number of (n+1)×4 binary arrays with each row and column not
   divisible by 3 (binary interpretation).  */
#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 81  /* 3^4 column states */

/* Static tables (initialised once) */
static uint8_t patterns[10];
static uint8_t next_state[STATE_COUNT][10];
static bool good_state[STATE_COUNT];
static bool initialized = false;

/* Initialise pattern list, transition table and final-state mask. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Build list of 4‑bit row patterns with integer value mod 3 ≠ 0 */
    int pcnt = 0;
    for (int pat = 0; pat < 16; ++pat) {
        if (pat % 3 != 0) {
            patterns[pcnt++] = (uint8_t)pat;
        }
    }

    /* Transition table: for each state (col values mod 3) and each pattern,
       compute next state after adding the row. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int a = s / 27;            /* exp(2) */
        int b = (s / 9) % 3;       /* exp(1) */
        int c = (s / 3) % 3;       /* exp(0) */
        int d = s % 3;

        good_state[s] = (a != 0 && b != 0 && c != 0 && d != 0);

        for (int p = 0; p < pcnt; ++p) {
            int bit0 = (patterns[p] >> 3) & 1;
            int bit1 = (patterns[p] >> 2) & 1;
            int bit2 = (patterns[p] >> 1) & 1;
            int bit3 = patterns[p] & 1;

            int na = (a * 2 + bit0) % 3;
            int nb = (b * 2 + bit1) % 3;
            int nc = (c * 2 + bit2) % 3;
            int nd = (d * 2 + bit3) % 3;

            int ns = na * 27 + nb * 9 + nc * 3 + nd;
            next_state[s][p] = (uint8_t)ns;
        }
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* Undefined for n<1, but return safe value */

    init();

    idx_t rows = n + 1;  /* Number of rows */

    /* DP arrays for current and next state counts */
    seq_t dp[STATE_COUNT] = {0};
    seq_t dp_next[STATE_COUNT] = {0};
    dp[0] = 1;   /* Start with all column values zero */

    for (idx_t r = 0; r < rows; ++r) {
        /* zero dp_next */
        for (int s = 0; s < STATE_COUNT; ++s) dp_next[s] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            for (int p = 0; p < 10; ++p) {
                int ns = next_state[s][p];
                dp_next[ns] += cur;
            }
        }

        /* move to next iteration */
        for (int s = 0; s < STATE_COUNT; ++s) dp[s] = dp_next[s];
    }

    /* Sum counts for final states with all column values non‑zero mod 3 */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (good_state[s]) {
            result += dp[s];
        }
    }

    return result;
}
