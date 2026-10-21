/*
 * A268524  a(n) = r*a(ceiling(n/2))+s*a(floor(n/2)) with a(1)=1 and
 *          (r,s)=(3,1).
 * https://oeis.org/A268524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(ceil(n/2)) + 1*a(floor(n/2)), with a(1)=1
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storage */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;  /* allocated elements 0 .. cache_cap-1 */
    static idx_t cache_len = 0;  /* highest initialized index (>=1) */

    if (n == 0)   /* offset starts at 1, return 0 for safety */
        return (seq_t)0;

    /* Ensure we have an array large enough for indices up to n. */
    if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);   /* allocation failure */
    }

    /* Initialise the first element if not already done. */
    if (cache_len < 1) {
        cache[1] = (seq_t)1;
        cache_len = 1;
    }

    /* Build values up to n using the recurrence. */
    while (cache_len < n) {
        idx_t i           = cache_len + 1;
        idx_t ceil_half   = (i + 1) / 2;
        idx_t floor_half  = i / 2;
        cache[i] = (seq_t)3 * cache[ceil_half] + cache[floor_half];
        cache_len = i;
    }

    return cache[n];
}
