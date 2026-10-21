/*
 * A283036  Number of n X 2 0..1 arrays with no 1 equal to more than one of
 *          its horizontal and vertical neighbors, with the exception of
 *          exactly one element.
 * https://oeis.org/A283036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 4*a(n-1) + 2*a(n-2) -12*a(n-3)
 *            -11*a(n-4) +4*a(n-5) +6*a(n-6) - a(n-8) Initial values (1‑based)
 *            a(1)=0 a(2)=4 a(3)=16 a(4)=68 a(5)=256 a(6)=924 a(7)=3232
 *            a(8)=11044 The implementation memoises the sequence and grows
 *            the cache as needed. The code is valid C11 and C++17
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;                 /* 1‑based storage: cache[i‑1] = a(i) */
static idx_t cache_len = 0;                 /* number of terms currently stored */
static const seq_t init_vals[8] = {
    (seq_t)0, (seq_t)4, (seq_t)16, (seq_t)68,
    (seq_t)256, (seq_t)924, (seq_t)3232, (seq_t)11044
};
static const idx_t init_len = 8;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for offset 0 */

    /* If requested term is already cached, return it */
    if (n <= cache_len) return cache[n-1];

    /* Allocate a new cache big enough for n terms */
    idx_t new_len = n;
    seq_t *new_cache = alloc_seq_array(new_len);
    if (!new_cache) return (seq_t)0;             /* allocation failure */

    /* Copy already known values to the new cache */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* If the new cache is not large enough to reach the recurrence’s
       initial segment, fill the initial segment directly. */
    if (new_len <= init_len) {
        for (idx_t i = 0; i < new_len; ++i)
            new_cache[i] = init_vals[i];
        /* Replace old cache and return */
        if (cache) free_seq_array(cache);
        cache = new_cache;
        cache_len = new_len;
        return cache[n-1];
    }

    /* Ensure the first eight values are set */
    if (cache_len < init_len) {
        for (idx_t i = 0; i < init_len; ++i)
            new_cache[i] = init_vals[i];
        cache_len = init_len;
    }

    /* Compute the remaining terms up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t j = i - 1; /* cache index for a(i) */
        new_cache[j] =
            (seq_t)4 * new_cache[j-1] +
            (seq_t)2 * new_cache[j-2] -
            (seq_t)12 * new_cache[j-3] -
            (seq_t)11 * new_cache[j-4] +
            (seq_t)4 * new_cache[j-5] +
            (seq_t)6 * new_cache[j-6] -
            new_cache[j-8];
    }

    /* Replace old cache with the enlarged one */
    if (cache) free_seq_array(cache);
    cache = new_cache;
    cache_len = new_len;

    return cache[n-1];
}
