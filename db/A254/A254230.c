/*
 * A254230  Heptagonal numbers (A000566) which are also centered square
 *          numbers (A001844).
 * https://oeis.org/A254230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+103682*a(n-2)-103682*a(n-3)-a(n-4)+a(n-5);
   initial values: a(1)=1, a(2)=4141, a(3)=85285, a(4)=429332905, a(5)=8842505113 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms already stored (a(1) .. a(cache_len)) */

    /* If the value is already cached, return it. */
    if (n <= cache_len) return cache[n-1];

    /* Expand the cache to hold n terms. */
    cache = (seq_t*)realloc(cache, sizeof(seq_t) * n);
    if (!cache) return 0;           /* allocation failed */

    cache_len = n;

    /* Initial five terms needed for the recurrence. */
    if (cache_len >= 1) cache[0] = (seq_t)1;
    if (cache_len >= 2) cache[1] = (seq_t)4141;
    if (cache_len >= 3) cache[2] = (seq_t)85285;
    if (cache_len >= 4) cache[3] = (seq_t)429332905;
    if (cache_len >= 5) cache[4] = (seq_t)8842505113;

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = 5; i < cache_len; ++i) {
        cache[i] = cache[i-1]
                 + (seq_t)103682 * cache[i-2]
                 - (seq_t)103682 * cache[i-3]
                 - cache[i-4]
                 + cache[i-5];
    }

    return cache[n-1];
}
