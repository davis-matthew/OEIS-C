/*
 * A369681  a(n) = Product_{k=0..n} (4^k + 5^(n-k)).
 * https://oeis.org/A369681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (4^k + 5^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for 5^i, i = 0..n */
    size_t len = (size_t)n + 1;
    seq_t *pow5 = (seq_t *)malloc(len * sizeof (seq_t));
    if (!pow5) return 0;                 /* OOM guard, value irrelevant */

    pow5[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow5[i] = pow5[i - 1] * 5;

    seq_t result = 1;
    seq_t pow4   = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow4 + pow5[n - k];
        result *= term;
        pow4   *= 4;
    }

    free(pow5);
    return result;
}
