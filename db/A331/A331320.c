/*
 * A331320  a(n) = [x^n] ((x + 1)*(2*x - 1)*(2*x^2 - 1))/(2*x^2 + 2*x - 1)^2.
 * https://oeis.org/A331320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) - 4*a(n-4) for n>4 a(0) = 1, a(1) =
 *            3, a(2) = 8, a(3) = 26, a(4) = 80
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoised values */
static idx_t cache_len = 0;      /* number of valid entries in cache */

/* Base values for n < 5 */
static const seq_t base_vals[5] = {1, 3, 8, 26, 80};

seq_t a(idx_t n)
{
    /* If the cache already holds n, return it */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache is large enough for n+1 entries */
    size_t new_len = (size_t)(n + 1);
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    } else if (new_len > cache_len) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    }
    /* Compute missing entries from cache_len up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 5) {
            cache[i] = base_vals[(size_t)i];
        } else {
            cache[i] = 4 * cache[i - 1] - 8 * cache[i - 3] - 4 * cache[i - 4];
        }
    }
    cache_len = new_len;
    return cache[n];
}
