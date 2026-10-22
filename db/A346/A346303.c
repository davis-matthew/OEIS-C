/*
 * A346303  Positions of words in A076478 that start with 0 and end with 0.
 * https://oeis.org/A346303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of words in A076478 that start with 0 and end with 0.
   Formula: Let L = floor(log2(n-1)) + 2 (for n > 1, L = 1 for n = 1).
   Then a(n) = 2^L - 1 + 2*(n - 2^(L-2) - 1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Compute L = floor(log2(n-1)) + 2 */
    idx_t t = n - 1;                       /* t > 0 */
    int k   = 63 - __builtin_clzll((unsigned long long)t);  /* floor(log2(t)) */
    idx_t L  = (idx_t)k + (idx_t)2;        /* length of the word block */

    /* Count of terms up to length L-1 is 2^(L-2) */
    seq_t prev_big = pow2(L - (idx_t)2);
    idx_t prev_    = (idx_t)prev_big;      /* fits in idx_t for n >= 2 */

    /* 1‑based offset within the current block */
    idx_t offset = n - prev_;

    /* Position of the first word of length L is 2^L - 1 */
    seq_t start  = pow2(L) - (seq_t)1;

    /* Final position: start + 2*(offset-1) */
    seq_t ans = start + ((seq_t)offset * (seq_t)2) - (seq_t)2;

    return ans;
}
