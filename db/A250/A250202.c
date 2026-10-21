/*
 * A250202  The "sum of squares of digits" problem in base 12, start with 6
 *          (written in base 10).
 * https://oeis.org/A250202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 6; a(n+1) = sum of squares of the base‑12 digits of a(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_sq_digits12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sum_sq_digits12.h"

/* Static cache for already computed terms. 0‑based indices. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure the cache can hold at least `need` elements (0‑based). */
static void ensure_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();   /* out of memory */

    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Public function as required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    size_t idx = (size_t)(n - 1);   /* zero‑based index */

    /* Allocate enough space and initialise the cache if empty. */
    ensure_cap(idx + 1);
    if (cache_len == 0) {
        cache[0] = 6;   /* a(1) */
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    while (cache_len <= idx) {
        seq_t prev = cache[cache_len - 1];
        cache[cache_len] = sum_sq_digits12(prev);
        ++cache_len;
    }
    return cache[idx];
}
