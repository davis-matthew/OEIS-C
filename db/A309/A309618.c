/*
 * A309618  a(n) = Sum_{k=0..floor(n/2)} k! * 2^k * (n - 2*k)!.
 * https://oeis.org/A309618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} k! * 2^k * (n - 2*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate factorial array up to n */
    seq_t *fact = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!fact) return 0; /* allocation failed */

    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t sum = 0;
    seq_t pow2 = 1;
    idx_t kmax = n / 2;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t term = fact[k] * pow2 * fact[n - 2 * k];
        sum += term;
        pow2 *= 2;
    }

    free(fact);
    return sum;
}
