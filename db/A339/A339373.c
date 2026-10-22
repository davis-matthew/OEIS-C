/*
 * A339373  Number of partitions of n into an even number of triangular
 *          numbers.
 * https://oeis.org/A339373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an even number of
 *            triangular numbers
 * Range:     u8 n <= 51, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system */
seq_t a(idx_t n)
{
    /* Dynamic programming arrays: even parts count and odd parts count */
    seq_t *even = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *odd  = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!even || !odd) return (seq_t)0;   /* fall back on error */

    even[0] = 1;   /* empty partition counts as even (zero parts) */

    /* Iterate over triangular numbers t_k = k*(k+1)/2 while t_k <= n */
    for (idx_t k = 1; ; ++k)
    {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;
        if (t > n) break;

        for (size_t j = (size_t)t; j <= (size_t)n; ++j)
        {
            /* Adding one part t flips parity; unlimited use of t is handled
               because we iterate j ascending and use already updated values. */
            even[j] += odd[j - (size_t)t];
            odd[j]  += even[j - (size_t)t];
        }
    }

    seq_t result = even[(size_t)n];

    free(even);
    free(odd);
    return result;
}
