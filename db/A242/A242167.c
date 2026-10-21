/*
 * A242167  Number of length n binary words that contain 000 and 001 and 010
 *          and 011 and 100 and 101 and 110 and 111 as contiguous
 *          subsequences. The 3 letter subsequences are allowed to overlap.
 * https://oeis.org/A242167
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over the set of seen
   3‑bit patterns and the last two bits of the word.  For each
   length n the algorithm counts how many binary words of that
   length contain all eight patterns.  The result is returned
   as a seq_t value. */

#include <stddef.h>   /* for size_t */

#define PATTERNS 8
#define MASK_ALL ((1 << PATTERNS) - 1)   /* 0xFF */
#define STATE_COUNT 256 * 4              /* (MASK_ALL+1) * 4 = 1024 */

seq_t a(idx_t n)
{
    /* Arrays for current and next DP layers */
    seq_t dp[STATE_COUNT];
    seq_t next[STATE_COUNT];

    /* initialise to zero */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)0;

    /* initial words of length 2: no 3‑bit pattern seen yet */
    for (int last2 = 0; last2 < 4; ++last2) {
        int idx = 0 * 4 + last2;   /* mask = 0 */
        dp[idx] = (seq_t)1;
    }

    /* build words of increasing length up to n */
    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = (seq_t)0;

        for (int mask = 0; mask <= MASK_ALL; ++mask) {
            for (int last2 = 0; last2 < 4; ++last2) {
                seq_t val = dp[mask * 4 + last2];
                if (val == (seq_t)0) continue;

                for (int bit = 0; bit < 2; ++bit) {
                    int new_last2 = ((last2 << 1) & 3) | bit;
                    int pattern   = (last2 << 1) | bit;       /* 0..7 */
                    int new_mask  = mask | (1 << pattern);
                    next[new_mask * 4 + new_last2] += val;
                }
            }
        }
        /* swap dp and next */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = next[i];
    }

    /* sum counts of states that have seen all patterns */
    seq_t result = (seq_t)0;
    for (int last2 = 0; last2 < 4; ++last2) {
        result += dp[MASK_ALL * 4 + last2];
    }

    return result;
}
