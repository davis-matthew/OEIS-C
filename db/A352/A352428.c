/*
 * A352428  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n,3*k+1) *
 *          a(n-3*k-1).
 * https://oeis.org/A352428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n,3k+1) * a(n-3k-1)
 *            with a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the binomial helper and standard headers */;
#include <stddef.h>  /* for size_t */
#include <stdlib.h>  /* for malloc, realloc */
#include "../utils/binomial.h"

/* Static cache for computed terms. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* number of allocated elements */
static idx_t max_computed = 0; /* highest index computed so far */

/* Ensure the cache has space for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    ensure_cache(n);

    /* Initialize a(0) if this is the first computation. */
    if (max_computed == 0 && cache[0] == 0)
        cache[0] = (seq_t)1;

    /* Compute any remaining terms up to n. */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t maxk = (i - 1) / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t choose = 3 * k + 1;
                seq_t bin = binomial(i, choose);         /* C(i,3k+1) */
                seq_t prod = bin * cache[i - choose];    /* * a(i-3k-1) */
                sum += prod;
            }
            cache[i] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
