/*
 * A288913  a(n) = Lucas(4*n + 3).
 * https://oeis.org/A288913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4*n+3) Recurrence: a(0) = 4, a(1) = 29, a(n) =
 *            7*a(n-1) - a(n-2) (n > 1) The sequence is memoised so that calls
 *            in any order are fast
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#  include <cstddef>
#  include <new>
#  include <cstdlib>
#else
#  include <stdlib.h>
#endif

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of already computed values   */
static size_t cache_cap = 0;   /* allocated capacity in elements       */

static void ensure_capacity_and_compute(idx_t n)
{
    /* Grow the storage if needed. */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *new_cache;
#if defined(__cplusplus)
        new_cache = static_cast<seq_t*>(::operator new(new_cap * sizeof(seq_t)));
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif

        /* Copy the existing elements into the new storage. */
        for (size_t i = 0; i < cache_len; ++i)
        {
#if defined(__cplusplus)
            new (&new_cache[i]) seq_t(cache[i]);   /* copy‑construct */
#else
            new_cache[i] = cache[i];
#endif
        }
        /* Default‑construct the remaining positions. */
        for (size_t i = cache_len; i < new_cap; ++i)
        {
#if defined(__cplusplus)
            new (&new_cache[i]) seq_t();
#else
            new_cache[i] = 0;
#endif
        }

        /* Destroy the old storage. */
        for (size_t i = 0; i < cache_len; ++i)
        {
#if defined(__cplusplus)
            cache[i].~seq_t();
#endif
        }
#if defined(__cplusplus)
        ::operator delete(cache);
#else
        free(cache);
#endif

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n (inclusive). */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)4;
        else if (i == 1)
            cache[1] = (seq_t)29;
        else
            cache[i] = (seq_t)7 * cache[i-1] - cache[i-2];
    }

    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if ((size_t)n < cache_len)
        return cache[n];
    ensure_capacity_and_compute(n);
    return cache[n];
}
