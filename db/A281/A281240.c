/*
 * A281240  Solutions y to the negative Pell equation y^2 = 72*x^2 - 83232
 *          with x,y >= 0.
 * https://oeis.org/A281240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n>6 Initial values: a(1)=0,
 *            a(2)=144, a(3)=480, a(4)=816, a(5)=1344, a(6)=3024
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 30, u64 n <= 68, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[7] = {0, 0, 144, 480, 816, 1344, 3024};

static seq_t *cache = NULL;
static idx_t cache_size = 0; /* highest index currently stored (0 means none) */

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* Allocate space up to n inclusive */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache) {
        /* On allocation failure, abort (should not happen for expected sizes) */
        abort();
    }
    cache = new_cache;

    /* Fill in values from current size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6];
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 but safe fallback */
    ensure_cache(n);
    return cache[n];
}
