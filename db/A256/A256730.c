/*
 * A256730  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum zero and no antidiagonal sum three.
 * https://oeis.org/A256730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of (n+2)×5 binary arrays with no 3×3 subblock
   whose main diagonal sums to 0 and no 3×3 subblock whose anti‑diagonal
   sums to 3.  The computation uses a dynamic program over states
   consisting of two consecutive rows.  For each pair of rows we
   pre‑compute the set of rows that can follow it without creating a
   forbidden subblock.  The total count is obtained by iterating H = n+2
   rows and accumulating the number of ways to reach each state. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Transition mask: for each pair (a,b) (encoded as (a<<5)|b)
   the bit i in trans_mask[s] is 1 iff row c=i may follow (a,b). */
static uint32_t trans_mask[1024];

/* Utility: check whether rows a,b,c violate any forbidden 3×3 subblock. */
static inline bool allowed_row(int a, int b, int c)
{
    for (int off = 0; off < 3; ++off) {
        /* main diagonal all zeros */
        if ( ((a >> off) & 1) == 0 &&
             ((b >> (off + 1)) & 1) == 0 &&
             ((c >> (off + 2)) & 1) == 0 )
            return false;
        /* anti‑diagonal all ones */
        if ( ((a >> (off + 2)) & 1) == 1 &&
             ((b >> (off + 1)) & 1) == 1 &&
             ((c >> off) & 1) == 1 )
            return false;
    }
    return true;
}

/* Initialise the transition masks once. */
static void init_transitions(void)
{
    static bool ready = false;
    if (ready) return;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c) {
                if (allowed_row(a, b, c))
                    mask |= (1u << c);
            }
            trans_mask[(a << 5) | b] = mask;
        }
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset, return 0 */

    const idx_t H = n + 2;         /* number of rows */

    init_transitions();

    /* DP arrays: dp and ndp hold counts for each state (pair of rows). */
    seq_t dp[1024];
    seq_t ndp[1024];

    /* initialise with all possible first two rows */
    for (int i = 0; i < 1024; ++i) dp[i] = (seq_t)1;

    /* Iterate through rows 3 .. H */
    for (idx_t r = 3; r <= H; ++r) {
        /* clear ndp */
        for (int i = 0; i < 1024; ++i) ndp[i] = (seq_t)0;

        for (int s = 0; s < 1024; ++s) {
            seq_t ways = dp[s];
            if (ways == (seq_t)0) continue;

            uint32_t mask = trans_mask[s];
            while (mask) {
                uint32_t bit = mask & -mask;
                int c    = __builtin_ctz(mask);
                mask ^= bit;
                int newState = (((s & 31) << 5) | c); /* (b<<5)|c */
                ndp[newState] += ways;
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < 1024; ++i) dp[i] = ndp[i];
    }

    /* Sum all terminal states to get the total number of arrays */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 1024; ++i) total += dp[i];

    return total;
}
