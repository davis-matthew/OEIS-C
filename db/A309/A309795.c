/*
 * A309795  Number of even parts appearing among the second largest parts of
 *          the partitions of n into 4 parts.
 * https://oeis.org/A309795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A309795 – Number of even parts appearing among the second
 *  largest parts of the partitions of n into 4 parts.
 *
 *  Formula (recurrence for n > 15):
 *      a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + 2*a(n-8)
 *             - 2*a(n-9) + a(n-10) - a(n-14) + 2*a(n-15) - a(n-16)
 *  Initial values a(0)..a(15) are hard‑coded below.
 */

#include <stdlib.h>

/* hard‑coded initial terms a(0) … a(15) */
static const seq_t init_val[16] = {
    (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)3, (seq_t)4, (seq_t)5,
    (seq_t)7, (seq_t)9, (seq_t)12, (seq_t)14
};

/* cache of computed values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;    /* size of allocated array (number of slots) */
static idx_t computed_max = 0; /* highest index fully computed */

/* ensure cache has space for indices 0 … n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len)
        return;

    /* enlarge to exactly n+1 entries (no extra growth) */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));
    if (!new_cache) abort();          /* out of memory – abort */
    cache = new_cache;
    cache_len = n + 1;
}

/* initialise cache with the first 16 terms */
static void init_cache(void)
{
    if (!cache || cache_len < 16) {
        ensure_cache(15);
    }
    for (idx_t i = 0; i < 16; ++i)
        cache[i] = init_val[i];
    computed_max = 15;
}

/*
 *  Main function requested by the test harness.
 *  Computes a(n) for any n >= 0, using the recurrence for n > 15.
 */
seq_t a(idx_t n)
{
    /* trivial case – use the hard‑coded table */
    if (n < 16) {
        if (!cache) init_cache();
        return cache[n];
    }

    /* initialise cache on first use */
    if (!cache) init_cache();

    /* enlarge cache to hold up to n */
    if (n + 1 > cache_len)
        ensure_cache(n);

    /* compute all missing values up to n */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            /* positive part of the recurrence */
            seq_t pos = (seq_t)2 * cache[i - 1]
                      + cache[i - 6]
                      + (seq_t)2 * cache[i - 8]
                      + cache[i - 10]
                      + (seq_t)2 * cache[i - 15];

            /* negative part of the recurrence */
            seq_t neg = cache[i - 2]
                      + (seq_t)2 * cache[i - 7]
                      + (seq_t)2 * cache[i - 9]
                      + cache[i - 14]
                      + cache[i - 16];

            cache[i] = pos - neg;    /* the difference is always non‑negative */
        }
        computed_max = n;
    }

    return cache[n];
}
