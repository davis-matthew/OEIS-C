/*
 * A390892  a(n) = (1/4) * Sum_{k>=0} (3/4)^k * Stirling2(n+k,k).
 * https://oeis.org/A390892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 * a(n) = -4*n*a(n-1) + 16*∑_{k=0}^{n-1} binomial(n,k+1) * a(k) * a(n-1-k)
 * (Same recurrence as given in OEIS A390892.)
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t* cache = NULL;
static idx_t cache_size = 0;

/* Ensure cache has space for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    seq_t* new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    cache = new_cache;
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    /* Guarantee at least one element (a(0) = 1). */
    ensure_cache(n);
    if (cache_size == 0) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        cache_size = 1;
    }
    if (cache[0] == 0)
        cache[0] = 1;  /* a(0) = 1 */

    /* Compute all terms up to n that are not yet cached. */
    for (idx_t i = 1; i <= n; ++i) {
        /* Sum ∑_{k=0}^{i-1} binomial(i,k+1)*a(k)*a(i-1-k) */
        seq_t sum = 0;
        seq_t b   = (seq_t)i;          /* binomial(i,1) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = b * cache[k] * cache[i - 1 - k];
            sum += prod;
            /* Update binomial(i, k+2) from binomial(i, k+1) */
            if (k + 1 < i) {
                seq_t num = (seq_t)(i - (k + 1));
                seq_t den = (seq_t)(k + 2);
                b = (b * num) / den;  /* exact division */
            }
        }

        seq_t term1 = 4 * (seq_t)i * cache[i - 1];
        seq_t term2 = 16 * sum;
        cache[i] = term2 - term1;  /* final value a(i) */
    }

    return cache[n];
}
