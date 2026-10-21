/*
 * A252388  Number of (4+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   empirical recurrence a(1) = 651, a(2) = 570, a(3) = 616, a(4) =
 *            744, a(5) = 1091, a(6) = 1475, a(7) = 1880, a(8) = 2734, a(9) =
 *            3712, a(10) = 4817, a(11) = 7023, a(12) = 9576, a(13) = 12499,
 *            a(14) = 18249; For n > 14: a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9)
 * Range:     u8 none, u16 n <= 18, u32 n <= 52, u64 n <= 121, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation assumes that the empirical recurrence holds for all n ≥ 1. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation buffer: cache[0] = a(1)          */
static idx_t cache_size = 0;     /* number of terms currently stored (n ≥ 1).      */

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;               /* nothing to do                      */

    /* Reallocate the buffer to hold 'n' terms.   */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!tmp) exit(1);                         /* fatal error: allocation failure   */
    cache = tmp;

    /* Initial values that are known.            */
    static const seq_t init[14] = {
        651, 570, 616, 744, 1091, 1475, 1880,
        2734, 3712, 4817, 7023, 9576, 12499, 18249
    };

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 14) {
            cache[i - 1] = init[i - 1];
        } else {
            /* a(i) = 4*a(i-3) - 4*a(i-6) + a(i-9) */
            cache[i - 1] = (seq_t)4 * cache[i - 4]
                         - (seq_t)4 * cache[i - 7]
                         +          cache[i -10];
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 0            */
    ensure_cache(n);
    return cache[n - 1];
}
