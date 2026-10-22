/*
 * A334656  a(n) is the number of words of length n on the alphabet {0,1,2}
 *          with the number of 0's plus the number of 1's congruent to the
 *          number of 2's modulo 3.
 * https://oeis.org/A334656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + 9*a(n-3) a(0) = 1, a(1) = 0, a(2) = 4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the tiny base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* Use a sliding window to avoid large temporary arrays. */
    seq_t prev3 = (seq_t)1;   /* a(n-3)  for the current iteration */
    seq_t prev2 = (seq_t)0;   /* a(n-2)  for the current iteration */
    seq_t prev1 = (seq_t)4;   /* a(n-1)  for the current iteration */
    seq_t ai = 0;             /* current term a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        ai = 6 * prev2 + 9 * prev3; /* a(i) = 6*a(i-2)+9*a(i-3) */

        /* Shift the window for the next iteration:
           prev3 <= prev2, prev2 <= prev1, prev1 <= ai. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = ai;
    }

    return ai;
}
