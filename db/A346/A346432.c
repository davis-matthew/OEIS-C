/*
 * A346432  a(0) = 1; a(n) = n! * Sum_{k=0..n-1} (n-k+1) * a(k) / k!.
 * https://oeis.org/A346432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n*a(n-1) - 2*n*(n-1)*a(n-2) for n>=3, with a(0)=1,
 *            a(1)=2, a(2)=14
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    size_t need = idx + 1;

    /* Allocate or expand cache as needed */
    if (cache == 0) {
        cache_cap = (need < 4) ? 4 : need;
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0; /* allocation failure */
    } else if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap *= 2;
        if (new_cap < 4) new_cap = 4;
#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        delete [] cache;
#else
        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Ensure base values are set */
    if (cache[0] == 0) cache[0] = 1;
    if (cache[1] == 0) cache[1] = 2;
    if (cache[2] == 0) cache[2] = 14;

    /* Compute up to n using the recurrence */
    for (size_t i = 3; i <= idx; ++i) {
        if (cache[i] != 0) continue; /* already computed */
        seq_t t1 = 4 * (seq_t)i * cache[i - 1];
        seq_t t2 = 2 * (seq_t)i * (i - 1) * cache[i - 2];
        cache[i] = t1 - t2;
    }

    return cache[idx];
}
