/*
 * A275518  Number of simplices in corner-cut triangulation of the n-cube.
 * https://oeis.org/A275518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2^(n-1) - n! + n! * Σ_{i=1..n} (2^(i-1)-1)/i!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact_n = factorial(n);

    /* compute 2^(n-1) */
    seq_t pow2_n_1 = pow2(n - 1);

    /* base term */
    seq_t result = 1 + pow2_n_1 - fact_n;

    /* sum over i */
    seq_t sum = 0;
    seq_t ratio = fact_n;  /* current n! / i! */

    for (idx_t i = 1; i <= n; ++i) {
        /* (2^(i-1) - 1) */
        seq_t term = pow2(i - 1) - 1;

        /* add ratio * term to sum */
        sum += ratio * term;

        /* update ratio to n! / (i+1)! for next iteration */
        if (i < n) {
            ratio /= (seq_t)(i + 1);
        }
    }

    return result + sum;
}
