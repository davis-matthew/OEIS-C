/*
 * A327629  Expansion of Sum_{k>=1} x^(k*(k + 1)/2) / (1 - x^(k*(k + 1)/2))^2.
 * https://oeis.org/A327629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors d of n such that n/d is a triangular
 *            number This implementation works for any seq_t and idx_t types
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Compute the sequence entry a(n).  n is 1‑based.  All arithmetic is   */
/* performed in seq_t, casting the idx_t value n to seq_t only when     */
/* required by the specification.                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, but return 0 for safety*/
        return (seq_t)0;

    /* Allocate an array of seq_t values; index 0 is unused. */
    seq_t *sum = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!sum)                   /* allocation failed; return 0 as fallback  */
        return (seq_t)0;

    /* Outer loop over triangular numbers T_k = k*(k+1)/2
       We iterate k from 1 while T_k <= n.                       */
    for (idx_t k = 1; ; ++k)
    {
        idx_t T = k * (k + 1) / 2;
        if (T > n) break;                        /* stop when beyond n   */

        /* For every multiple m = q * T (q >= 1), add the divisor
           d = m / T = q to sum[m].                            */
        for (idx_t m = T; m <= n; m += T)
        {
            seq_t d = (seq_t)(m / T);            /* integer division   */
            sum[m] += d;
        }
    }

    seq_t result = sum[(size_t)n];
    free(sum);
    return result;
}
