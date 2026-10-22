/*
 * A350146  Partial sums of A002131.
 * https://oeis.org/A350146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A002131(k) with A002131(k) = sum_{d|k, k/d
 *            odd} d which equals 2^{v2(k)} * sigma(k/2^{v2(k})) where sigma
 *            is sum of divisors
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* safety for unexpected input */

    /* Allocate array for sum‑of‑divisors values (sigma) up to n. */
    seq_t *sigma = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
    if (!sigma) return 0;                    /* allocation failure (unlikely) */

    for (idx_t i = 1; i <= n; ++i) sigma[i] = 0;

    /* Classic divisor‑sieve: for each i add i to all multiples of i. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    seq_t sum = 0;

    /* Sum A002131(k) for k = 1..n */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t temp = k;
        idx_t v = 0;
        /* Find exponent of 2 in k (v2(k)). */
        while ((temp & 1) == 0) {
            ++v;
            temp >>= 1;
        }
        /* temp is now the odd part k / 2^v. */
        seq_t term = ((seq_t)1 << v) * sigma[temp];
        sum += term;
    }

    free(sigma);
    return sum;
}
