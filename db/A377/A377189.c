/*
 * A377189  Expansion of 1/(1 - 4*x^2 - 4*x^3)^(5/2).
 * https://oeis.org/A377189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*(n+3)*a(n-2) + 2*(2*n+9)*a(n-3)) / n Initial values:
 *            a(0) = 1, a(1) = 0, a(2) = 10 Recurrence applies for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* realloc, abort */

static seq_t *cache = NULL;   /* memoization array */
static idx_t  cache_size = 0; /* number of elements stored */

/* Computes a(n) for sequence A377189 */
seq_t a(idx_t n)
{
    /* Allocate enough storage to hold indices 0..n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();  /* out of memory */

        /* Compute missing entries from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)0;
            } else if (i == 2) {
                cache[2] = (seq_t)10;
            } else {
                /* a(i) = (4*(i+3)*a(i-2) + 2*(2*i+9)*a(i-3)) / i */
                seq_t term1 = cache[i-2];
                term1 = (seq_t)4 * ((seq_t)(i + 3) * term1);

                seq_t term2 = cache[i-3];
                term2 = (seq_t)2 * ((seq_t)(2 * i + 9) * term2);

                cache[i] = (term1 + term2) / (seq_t)i;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
