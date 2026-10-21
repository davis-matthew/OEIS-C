/*
 * A291388  a(n) = (1/8)*A291387(n).
 * https://oeis.org/A291388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(2)=44, a(3)=262, a(4)=1504
   a(n) = 8*a(n-1) - 8*a(n-2) - 32*a(n-3) - 16*a(n-4)  (n >= 5) */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache      = NULL;  /* memoised values */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t   next_to_compute = 0;/* next index to be filled */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
#endif

    /* Copy existing computed terms. */
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];

    /* Free the old buffer. */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Compute the n-th term of sequence A291388. */
seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > cache_cap) ensure_cache_capacity(need);

    while (next_to_compute <= n)
    {
        idx_t i   = next_to_compute;
        seq_t val;

        if (i == 0)        val = 1;
        else if (i == 1)   val = 7;
        else if (i == 2)   val = 44;
        else if (i == 3)   val = 262;
        else if (i == 4)   val = 1504;
        else
        {
            /* Compute 8*(a(i-1) - a(i-2) - 4*a(i-3) - 2*a(i-4)). */
            seq_t tmp = cache[i-1];
            tmp -= cache[i-2];
            tmp -= cache[i-3] << 2;  /* times 4 */
            tmp -= cache[i-4] << 1;  /* times 2 */
            tmp <<= 3;               /* times 8 */
            val = tmp;
        }

        cache[i] = val;
        ++next_to_compute;
    }

    return cache[n];
}
