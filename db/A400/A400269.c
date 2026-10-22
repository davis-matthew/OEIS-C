/*
 * A400269  G.f. A(x) satisfies A(x) = (1 + x) * A(x^3 * (1 + x)^4), with A(0)
 *          = 1.
 * https://oeis.org/A400269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(4*k+1, n-3*k) * a(k) with a(0)
 *            = 1
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Global cache for the sequence values */
static seq_t *a_cache = NULL;
static idx_t   cache_size = 0;
static idx_t   cache_computed_up_to = 0;

/* Ensure the cache is allocated and large enough for indices up to n.
   All entries beyond cache_computed_up_to are initialized to 0. */
static void a_cache_ensure(idx_t n)
{
    if (a_cache == NULL) {
        /* Allocate space for indices 0..n */
        cache_size = n + 1;
        a_cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        /* set a(0) */
        a_cache[0] = (seq_t)1;
        cache_computed_up_to = 0;
    } else if (n + 1 > cache_size) {
        /* Reallocate to a larger size */
        cache_size = n + 1;
        a_cache = (seq_t *)realloc(a_cache, cache_size * sizeof(seq_t));
    }
}

/* Compute the sequence values up to n if they are not yet known. */
static void a_compute_up_to(idx_t n)
{
    a_cache_ensure(n);
    if (cache_computed_up_to >= n)
        return;

    for (idx_t i = cache_computed_up_to + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t maxk = i / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t choose_n = i - 3 * k;
            seq_t bin = binomial(4 * k + 1, choose_n);
            sum += bin * a_cache[k];
        }
        a_cache[i] = sum;
    }
    cache_computed_up_to = n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    a_compute_up_to(n);
    return a_cache[n];
}
