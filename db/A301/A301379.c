/*
 * A301379  Number of close American football games: number of ways for the
 *          game to have n scoring plays and never be separated by more than
 *          one score after each play.
 * https://oeis.org/A301379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301379: number of close American football games with n scoring plays.
   Each play changes the score difference by ±k (2 ≤ k ≤ 8).  The walk is
   constrained to the interval [-8,8] (17 possible positions).
   Recurrence:
       let cur[i] = number of walks ending at position i-8 after a given number of plays
       cur[8] = 1  (empty walk)
       for each step:
           next[j] = Σ_{k=2..8} cur[j-k] + cur[j+k]   (indices clipped to 0..16)
   a(n) = Σ_{i=0}^{16} cur[i] after n steps.
   a(0) = 1, a(1) = 14, a(2) = 128, … */

#include <stddef.h>

#define OFFSET   8
#define STATES  17

seq_t a(idx_t n)
{
    seq_t cur[STATES];
    seq_t next[STATES];
    int i, k;

    /* initialise to zero */
    for (i = 0; i < STATES; ++i) cur[i] = 0;
    cur[OFFSET] = 1;                 /* empty walk */

    for (idx_t step = 1; step <= n; ++step) {
        for (i = 0; i < STATES; ++i) next[i] = 0;

        for (i = 0; i < STATES; ++i) {
            seq_t val = cur[i];
            if (!val) continue;
            for (k = 2; k <= 8; ++k) {
                int d = i + k;
                if (d < STATES) next[d] += val;
                d = i - k;
                if (d >= 0)    next[d] += val;
            }
        }

        /* copy next into cur for next iteration */
        for (i = 0; i < STATES; ++i) cur[i] = next[i];
    }

    seq_t total = 0;
    for (i = 0; i < STATES; ++i) total += cur[i];
    return total;
}
