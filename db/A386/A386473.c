/*
 * A386473  a(0) = 1; a(n) = a(n-1) - Sum_{k=0..n-1} k * a(k) * a(n-1-k).
 * https://oeis.org/A386473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 15, i64 n <= 23, i128 n <= 1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A386473
 *
 * Recurrence:
 *   a(0) = 1
 *   a(n) = a(n-1) - Sum_{k=0..n-1} k * a(k) * a(n-1-k)
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache for computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache can hold terms up to n. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;
    /* Double the size until it fits. */
    size_t new_cap = cache_size ? cache_size * 2 : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* out of memory */
    cache = tmp;
    cache_size = new_cap;
    /* Initialise new entries to 0. */
    for (size_t i = n + 1; i < new_cap; ++i)
        cache[i] = 0;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;              /* a(0) = 1 */

    ensure_cache(n);

    if (cache[n] != 0)
        return cache[n];       /* already computed */

    /* Ensure a(0) is set for recurrence start. */
    if (cache[0] == 0)
        cache[0] = 1;

    /* Compute a(n) incrementally up to the requested index. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)
            continue;          /* already computed earlier call */

        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = (seq_t)k;          /* cast k to seq_t */
            term *= cache[k];
            term *= cache[i - 1 - k];
            sum += term;
        }
        cache[i] = cache[i - 1] - sum;
    }

    return cache[n];
}
