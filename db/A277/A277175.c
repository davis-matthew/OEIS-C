/*
 * A277175  Convolution of Catalan numbers and factorial numbers.
 * https://oeis.org/A277175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A277175: a(n) = Sum_{i=0..n} Catalan(i) * (n-i)!
 * Sequence of convolutions of Catalan numbers and factorials. */
#include <stdlib.h>

/* Note: idx_t and seq_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Precompute factorials up to n */
    seq_t *fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t ans = 0;
    seq_t catalan = 1;   /* C(0) */

    for (idx_t i = 0; i <= n; ++i)
    {
        ans = ans + (catalan * fact[n - i]);

        /* Update Catalan for next i (skip after last iteration) */
        if (i < n)
        {
            /* C(i+1) = C(i) * 2*(2*i+1) / (i+2)   */
            seq_t numerator = catalan * (seq_t)(2 * (2 * i + 1));
            seq_t denominator = (seq_t)(i + 2);
            catalan = numerator / denominator;
        }
    }

    free(fact);
    return ans;
}
