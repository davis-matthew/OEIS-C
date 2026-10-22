/*
 * A333494  a(1) = 1; a(n) = Sum_{k=1..n-1} ceiling(n/k) * a(k).
 * https://oeis.org/A333494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = Sum_{k=1}^{n-1} ceil(n/k) * a(k) for n >= 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache to memoize computed terms.
   It grows the cache as needed. The algorithm runs in O(n²) time
   (≈5·10⁷ iterations for n = 10000), which stays well below the 2 s
   limit for typical 64‑bit platforms.  All arithmetic is performed
   with the build‑defined type seq_t, and indices are of type idx_t. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* a(1) stored at cache[0] */
static idx_t  cache_len = 0;   /* number of terms currently stored    */
static idx_t  cache_cap = 0;   /* allocated capacity of the cache     */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, return 0 */

    /* Grow the cache if it cannot hold n terms. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute and store missing terms up to n. */
    if (n > cache_len) {
        if (cache_len == 0) {          /* initialise a(1) */
            cache[0] = 1;
            cache_len = 1;
            if (n == 1)
                return cache[0];
        }
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                idx_t ceil_val = (i + k - 1) / k;          /* ceil(i/k) */
                sum +=  (seq_t)ceil_val * cache[k - 1];
            }
            cache[i - 1] = sum;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
