/*
 * A258555  Number of (2+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) + 3*a(n-6) - a(n-7)
   for n > 10.
   Initial values:
   a(1)=44, a(2)=112, a(3)=210, a(4)=344, a(5)=506,
   a(6)=683, a(7)=894, a(8)=1140, a(9)=1421, a(10)=1725.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, behaviour for n < 1 not important */

    /* First ten terms, for n = 1..10  */
    static const seq_t init[10] = {
        44, 112, 210, 344, 506,
        683, 894, 1140, 1421, 1725
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* points to array of length cache_len */
    static idx_t cache_len = 0;   /* number of terms stored (cache_len >= 10 after init) */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_len = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return 0;   /* out of memory; return 0 as error value */
        for (idx_t i = 0; i < cache_len; ++i) {
            cache[i] = init[i];
        }
    }

    /* If requested term already cached, return it */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend cache to contain term at index n */
    idx_t new_len = n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL) return 0;   /* out of memory; return 0 */
    cache = tmp;
    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < new_len; ++i) {
        /* i is 0‑based index for a(i+1), we need i-1 .. i-7 */
        seq_t pos = 3*cache[i-1] + cache[i-3] + cache[i-4] + 3*cache[i-6];
        seq_t neg = 3*cache[i-2] + 3*cache[i-5] + cache[i-7];
        cache[i] = pos - neg;   /* recurrence guarantees non‑negative result */
    }
    cache_len = new_len;

    return cache[n - 1];
}
