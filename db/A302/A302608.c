/*
 * A302608  a(n) = n! * [x^n] exp(n*x)*arctan(x).
 * https://oeis.org/A302608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, k odd} (-1)^((k-1)/2) * binomial(n,k) *
 *            (k-1)! * n^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes a(n) for OEIS A302608.  The function is safe for any n >= 0
 * and works with seq_t being 8, 16, 32, 64, 128-bit unsigned integers
 * or an arbitrary‑precision integer class.  All intermediate arithmetic
 * is performed in seq_t, avoiding floating point or large temporary
 * arrays. */
seq_t a(idx_t n)
{
    /* Handle n = 0: the sequence is defined as 0. */
    if (n == 0u)
        return 0u;

    seq_t result = 0u;            /* Accumulator for the sum. */
    seq_t bin = 1u;               /* Current binomial C(n, k-1). */
    seq_t fact = 1u;              /* (k-1)! for current k.          */
    seq_t pow = 1u;               /* n^(n-k) for current k.         */

    /* Compute initial power: n^(n-1) .  For n=1 this stays 1. */
    /* The loop runs (n-1) times; for 64‑bit builds n-1 <= 15 fits. */
    for (idx_t i = 1; i < n; ++i)
        pow *= (seq_t)n;

    /* Iterate k = 1 .. n.  bin and fact are updated at the start of
     * each loop to give C(n, k) and (k-1)! respectively.      */
    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient C(n, k).  The division is exact. */
        bin = bin * (seq_t)(n - k + 1) / (seq_t)k;

        /* Only odd k contribute to the sum. */
        if ((k & 1u) != 0u) {
            seq_t term = bin * fact * pow;

            /* Determine sign: (-1)^((k-1)/2).  The parity of (k-1)/2
             * is simply (k > 1 ? ((k-1)/2) & 1 : 0). */
            bool neg = ((k - 1) >> 1) & 1u;
            if (neg)
                result -= term;
            else
                result += term;
        }

        /* Prepare (k)! for the next iteration. */
        fact *= (seq_t)k;

        /* Prepare n^(n-(k+1)) for the next iteration.  The division is
         * exact because we are effectively removing one factor of n. */
        pow /= (seq_t)n;
    }

    return result;
}
