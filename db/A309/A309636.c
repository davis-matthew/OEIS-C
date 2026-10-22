/*
 * A309636  a(1) = 3, a(2) = 1, a(3) = 4, a(4) = 2, a(5) = 5; a(6) = 3; a(n) =
 *          a(n-a(n-1)) + a(n-a(n-4)) for n > 6.
 * https://oeis.org/A309636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 9262, u32 n <= 9262, u64 n <= 9262,
 *            u128 n <= 9262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A309636:  a(1)=3, a(2)=1, a(3)=4, a(4)=2, a(5)=5, a(6)=3;
 * for n>6: a(n)=a(n-a(n-1)) + a(n-a(n-4)).  Values are positive;
 * 0 is used as the cache sentinel because the sequence never produces 0. */

#include <stdlib.h>
#include <string.h>

/* Global cache for memoisation. */
static seq_t *cache = NULL;   /* 1‑based index, cache[0] unused */
static idx_t cache_size = 0;  /* current allocated size */

/* Resize the cache so that it can hold indices up to at least n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    idx_t new_size = (cache_size > 0) ? cache_size << 1 : 16;
    while (new_size < n)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)calloc((size_t)new_size + 1, sizeof(seq_t));
    if (!new_cache)
        return;  /* allocation failed – caller will work with 0 values */

    if (cache != NULL) {
        memcpy(new_cache, cache, (size_t)(cache_size + 1) * sizeof(seq_t));
        free(cache);
    }

    cache = new_cache;
    cache_size = new_size;
}

/* Recursive helper that uses the cache. */
static seq_t compute_a(idx_t n)
{
    /* One‑time initialisation of the first six terms. */
    if (cache == NULL) {
        ensure_cache(6);
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)3;
    }

    if (n <= cache_size && cache[n] != (seq_t)0)
        return cache[n];

    /* Compute recursively and store in cache. */
    if (n > cache_size)
        ensure_cache(n);

    /* Indices for the recurrence.  The sequence is defined for n>6. */
    idx_t idx1 = n - (idx_t)compute_a(n - 1);
    idx_t idx2 = n - (idx_t)compute_a(n - 4);

    seq_t val1 = compute_a(idx1);
    seq_t val2 = compute_a(idx2);
    seq_t result = val1 + val2;

    cache[n] = result;
    return result;
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    return compute_a(n);
}
