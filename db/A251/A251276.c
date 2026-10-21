/*
 * A251276  Number of (n+1) X (1+1) 0..3 arrays with no 2 X 2 subblock having
 *          its maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A251276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 26*a(n-2) for n >= 3 * Initial values: a(1) =
 *            230, a(2) = 3264
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>   /* malloc, free, realloc */
#include <stdint.h>   /* uint64_t for size_t etc. */

static seq_t *cache = NULL;          /* array of computed terms (0‑based) */
static size_t cache_len = 0;          /* number of valid entries in cache */
static size_t cache_cap = 0;          /* currently allocated size */

static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 4;
    while (new_cap < need)
        new_cap <<= 1;           /* grow geometrically */

#ifdef __cplusplus
    seq_t *new_buf = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        new_buf[i] = cache[i];
    delete[] cache;
    cache = new_buf;
#else
    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf) return;        /* OOM – unlikely in test harness */
    for (size_t i = 0; i < cache_len; ++i)
        new_buf[i] = cache[i];
    free(cache);
    cache = new_buf;
#endif
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* undefined region */

    /* Target index in 0‑based array */
    size_t target = (size_t)n;

    if (target > cache_len)
    {
        /* Ensure array capacity */
        ensure_capacity(target);

        /* If nothing in cache yet, initialise a(1) and a(2) */
        if (cache_len == 0)
        {
            cache[0] = (seq_t)230;
            cache_len = 1;
            /* a(2) will be handled below if needed */
        }

        /* If we only have a(1) but need a(2), initialise it now */
        if (cache_len == 1 && target >= 2)
        {
            cache[1] = (seq_t)3264;
            cache_len = 2;
        }

        /* Compute remaining terms using the recurrence */
        for (size_t i = cache_len + 1; i <= target; ++i)
        {
            /* i is 1‑based: a(i) depends on a(i‑1) and a(i‑2) */
            seq_t val = (seq_t)16 * cache[i - 2] - (seq_t)26 * cache[i - 3];
            cache[i - 1] = val;
        }
        cache_len = target;
    }

    return cache[n - 1];
}
