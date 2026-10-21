/*
 * A280475  Number of nX3 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A280475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 303*a(n-2) + 1857*a(n-3) - 6924*a(n-4) * +
 *            16608*a(n-5) - 26048*a(n-6) + 26112*a(n-7) - 15360*a(n-8) * +
 *            4096*a(n-9) * Initial values (n = 1..10): * 0, 22, 208, 1939,
 *            17406, 149057, 1224092, 9717407, 75078490, 567486429
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)0,
    (seq_t)22,
    (seq_t)208,
    (seq_t)1939,
    (seq_t)17406,
    (seq_t)149057,
    (seq_t)1224092,
    (seq_t)9717407,
    (seq_t)75078490,
    (seq_t)567486429
};

static seq_t *cache = NULL;   /* array of computed a(k), index 0 corresponds to a(1) */
static size_t cache_cap = 0;  /* current capacity of cache */

/* Extend the cache to hold at least n terms (indices 1..n) */
static void extend_cache(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;

    size_t new_cap = (size_t)n;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) {
        /* Allocation failure – terminate; callers expect a valid result. */
        abort();
    }

    for (size_t i = cache_cap; i < new_cap; ++i) {
        idx_t idx = (idx_t)(i + 1);               /* sequence index */
        if (idx <= 10) {
            cache[i] = init_vals[idx - 1];
        } else {
            /* Apply the empirical recurrence.  All operations are performed
             * using seq_t so that the computation is correct for all planned
             * sizes of seq_t (including arbitrary‑precision builds). */
            seq_t val = (seq_t)27   * cache[i - 1]
                       - (seq_t)303  * cache[i - 2]
                       + (seq_t)1857 * cache[i - 3]
                       - (seq_t)6924 * cache[i - 4]
                       + (seq_t)16608 * cache[i - 5]
                       - (seq_t)26048 * cache[i - 6]
                       + (seq_t)26112 * cache[i - 7]
                       - (seq_t)15360 * cache[i - 8]
                       + (seq_t)4096  * cache[i - 9];
            cache[i] = val;
        }
    }

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* Undefined for offset 1, but safe to return 0 */

    extend_cache(n);
    return cache[(size_t)(n - 1)];
}
