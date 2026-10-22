/*
 * A385252  Number of ternary strings of length 2*n that have at least one 0
 *          but less 0's than the combined number of 1's and 2's.
 * https://oeis.org/A385252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n - 4^n - Sum_{j=0..n} 2^j * C(2*n, j) with a(0) = 0,
 *            a(1) = 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                   /* for any arbitrary‑precision build */
#include "../utils/big_pow.h"          /* big_pow(base, exp)  -> seq_t */

seq_t a(idx_t n)
{
    /* a(0) and a(1) are defined as 0 in the OEIS entry */
    if (n < 2) return 0;

    /* Compute 9^n and 4^n */
    seq_t pow9 = big_pow((seq_t)9, n);
    seq_t pow4 = big_pow((seq_t)4, n);

    /* Compute the partial sum Σ_{j=0..n} 2^j * C(2*n, j) */
    seq_t term = (seq_t)1;   /* C(2n,0) * 2^0 */
    seq_t sum  = term;       /* include j = 0 term */

    for (idx_t j = 1; j <= n; ++j) {
        /* term_{j} = term_{j-1} * (2*n - (j-1)) / j * 2 */
        term *= (seq_t)(2 * n - (j - 1));
        term /= (seq_t)j;
        term *= (seq_t)2;
        sum += term;
    }

    /* Final result */
    return pow9 - pow4 - sum;
}
