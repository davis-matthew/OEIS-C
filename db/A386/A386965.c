/*
 * A386965  Number of permutations p of [2*n] such that there is at least one
 *          index i in [2*n-1] with p(i+1) = n + p(i).
 * https://oeis.org/A386965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(n,k) * (2*n - k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Below offset, return 0 */

    /* Allocate factorial array up to 2*n */
    idx_t size = 2 * n + 1;
    seq_t *fact = (seq_t *)malloc(sizeof(seq_t) * size);
    if (!fact) return 0; /* Out of memory */

    fact[0] = 1;
    for (idx_t i = 1; i <= 2 * n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* Accumulate positive and negative terms separately */
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binomial(n, k);
        seq_t term = bin * fact[2 * n - k];
        if ((k & 1) == 1) {
            pos += term;
        } else {
            neg += term;
        }
    }

    seq_t result = pos - neg;
    free(fact);
    return result;
}
