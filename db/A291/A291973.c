/*
 * A291973  a(n) = (3*n)! * [z^(3*n)] exp(exp(z)/3 +
 *          2*exp(-z/2)*cos(z*sqrt(3)/2)/3 - 1).
 * https://oeis.org/A291973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A291973:  a(n) = number of set partitions of {1..3n}
   where every block size is a multiple of 3.
   Recurrence: a(0) = 1; a(n) = Σ_{k=1..n} C(3n-1,3k-1) * a(n-k) */
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/seq_realloc.h"

/* Global cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure cache has space up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    if (!new_cache) abort();

    /* Initialise new entries to zero (uncomputed) */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;

    /* Base term */
    cache[0] = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    /* Compute values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        idx_t N = 3 * i - 1;            /* 3i - 1 */
        seq_t coeff = binomial(N, 2);   /* C(3i-1,2) for k=1 */
        idx_t r = 2;                    /* current r */

        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            /* term a(i-k) * C(3i-1, 3k-1) */
            sum += coeff * cache[i - k];

            if (k < i) {
                /* Next coefficient: C(N, r+3) from C(N, r) */
                idx_t next_r = r + 3;

                seq_t num1 = (seq_t)(N - r);
                seq_t num2 = (seq_t)(N - r - 1);
                seq_t num3 = (seq_t)(N - r - 2);
                seq_t den1 = (seq_t)(r + 1);
                seq_t den2 = (seq_t)(r + 2);
                seq_t den3 = (seq_t)(r + 3);

                seq_t numer = coeff * num1 * num2 * num3;
                seq_t denom = den1 * den2 * den3;
                coeff = numer / denom;   /* exact division */

                r = next_r;
            }
        }
        cache[i] = sum;
    }
    return cache[n];
}
