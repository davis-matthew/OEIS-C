/*
 * A294354  a(1) = a(3) = a(4) = 2, a(2) = 4; a(n) = a(n-a(n-1)) + a(n-a(n-2))
 *          for n > 4.
 * https://oeis.org/A294354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 2, a(2) = 4, a(3) = 2, a(4) = 2;
 *        a(n) = a(n - a(n-1)) + a(n - a(n-2))  for n > 4
 */
#include <stdlib.h>

static seq_t *cache   = NULL;          /* 1‑based array, index 0 unused */
static idx_t  cache_cap = 0;           /* number of allocated slots   */
static idx_t  cache_len = 0;           /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset is 1; below that is undefined */

    /* Grow the cache if needed */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms only */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1)      val = 2;
        else if (k == 2) val = 4;
        else if (k == 3) val = 2;
        else if (k == 4) val = 2;
        else {
            seq_t a1 = cache[k - 1];
            seq_t a2 = cache[k - 2];
            idx_t i = k - (idx_t)a1;
            idx_t j = k - (idx_t)a2;
            val = cache[i] + cache[j];
        }
        cache[k] = val;
    }
    cache_len = n;
    return cache[n];
}
