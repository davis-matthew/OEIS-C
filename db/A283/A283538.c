/*
 * A283538  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2) -7*a(n-3) +10*a(n-4) +4*a(n-5)
 *            -8*a(n-6) for n > 6 with base values a(1)=7, a(2)=27, a(3)=127,
 *            a(4)=573, a(5)=2615, a(6)=11903. The function is memoised to
 *            allow arbitrary order of indices
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* length currently allocated (1‑based, index 0 unused) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1 */

    /* Ensure the cache can hold indices up to n */
    if (n + 1 > cache_len) {
        /* Allocate new cache with enough space */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed */

        /* Copy any previously computed values into the new cache */
        if (cache && cache_len) {
            for (idx_t i = 1; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        cache = new_cache;
        cache_len = n + 1;
    }

    /* Initialise base values if not already present */
    if (n >= 1 && cache[1] == 0) cache[1] = 7;
    if (n >= 2 && cache[2] == 0) cache[2] = 27;
    if (n >= 3 && cache[3] == 0) cache[3] = 127;
    if (n >= 4 && cache[4] == 0) cache[4] = 573;
    if (n >= 5 && cache[5] == 0) cache[5] = 2615;
    if (n >= 6 && cache[6] == 0) cache[6] = 11903;

    /* Compute any missing terms up to n */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[i] == 0) {
            cache[i] = 5 * cache[i-1] - cache[i-2] - 7 * cache[i-3] +
                       10 * cache[i-4] + 4 * cache[i-5] - 8 * cache[i-6];
        }
    }

    return cache[n];
}
