/*
 * A291444  Number of symmetric maximal irredundant sets in the n-path graph.
 * https://oeis.org/A291444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) + a(n-6) + a(n-8) + a(n-10) - a(n-14) - 2*a(n-16)
 *            - a(n-18) + 2*a(n-20) + a(n-22) - a(n-28), n > 28 with the
 *            initial values a(1)=1, a(2)=0, a(3)=2, a(4)=2, a(5)=2, a(6)=2,
 *            a(7)=3, a(8)=3, a(9)=5, a(10)=6, a(11)=7, a(12)=6, a(13)=10,
 *            a(14)=12, a(15)=15, a(16)=15, a(17)=22, a(18)=23,...
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 116, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[29] = {
    0,   /* unused a(0) */
    1, 0, 2, 2, 2, 2, 3, 3, 5, 6, 7, 6, 10, 12, 15, 15, 22,
    23, 33, 35, 48, 48, 71, 75, 103, 106, 152, 158
};

static seq_t *cache          = NULL;
static idx_t  cache_size     = 0;   /* number of allocated slots */
static idx_t  computed_up_to = 0;   /* largest n that is already cached */

/* Grow the cache so that it can hold entries up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;               /* initialise new slots to zero */
        cache      = new_cache;
        cache_size = new_size;
    }
}

/* Initialise the base values a(1)…a(28) in the cache if not already done. */
static void init_cache(void)
{
    if (cache_size < 29)
        ensure_cache(28);
    if (computed_up_to < 28) {
        for (idx_t i = 1; i <= 28; ++i)
            cache[i] = init_vals[i];
        computed_up_to = 28;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)     /* undefined for the problem, return 0 safely */
        return (seq_t)0;

    if (n <= 28)            /* direct hit for the first 28 terms */
        return init_vals[n];

    init_cache();           /* guarantees a(1)…a(28) are present */

    ensure_cache(n);        /* make sure we have room for a(n) */

    /* Compute all missing values up to n (in linear time). */
    for (idx_t k = computed_up_to + 1; k <= n; ++k) {
        seq_t val =
            cache[k - 4] + cache[k - 6] + cache[k - 8] + cache[k - 10]
          - cache[k - 14] - cache[k - 16] * 2 - cache[k - 18]
          + cache[k - 20] * 2 + cache[k - 22] - cache[k - 28];
        cache[k] = val;
    }
    computed_up_to = n;
    return cache[n];
}
