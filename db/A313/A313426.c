/*
 * A313426  Coordination sequence Gal.5.220.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-2) - a(n-4) + 2*a(n-5) - a(n-6) + 2*a(n-7) +
 *            2*a(n-9) - a(n-10) + 2*a(n-11) - a(n-12) - a(n-14) - a(n-16) for
 *            n > 16 Initial values: a(0)=1, a(1)=5, a(2)=10, a(3)=13,
 *            a(4)=19, a(5)=26, a(6)=29, a(7)=35, a(8)=40, a(9)=43, a(10)=48,
 *            a(11)=51, a(12)=60, a(13)=67, a(14)=67, a(15)=72, a(16...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to store computed values between calls. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache contains values up to index n; compute missing
   values using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already computed */

    /* Allocate new cache of size n+1. */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!new_cache) exit(EXIT_FAILURE);

    /* Initial values for indices 0..16. */
    static const seq_t init_arr[17] = {
        1, 5, 10, 13, 19, 26, 29, 35, 40, 43,
        48, 51, 60, 67, 67, 72, 77
    };
    idx_t i;
    for (i = 0; i <= n && i < 17; ++i)
        new_cache[i] = init_arr[i];

    /* Compute new values using the recurrence. */
    for (; i <= n; ++i)
    {
        /* For indices < 17, the value was already set from init_arr. */
        if (i < 17) continue;

        seq_t res = 0;
        /* Positive contributions first. */
        res += new_cache[i - 5] * (seq_t)2;
        res += new_cache[i - 7] * (seq_t)2;
        res += new_cache[i - 9] * (seq_t)2;
        res += new_cache[i - 11] * (seq_t)2;
        /* Negative contributions. */
        res -= new_cache[i - 2];
        res -= new_cache[i - 4];
        res -= new_cache[i - 6];
        res -= new_cache[i - 10];
        res -= new_cache[i - 12];
        res -= new_cache[i - 14];
        res -= new_cache[i - 16];
        new_cache[i] = res;
    }

    /* Replace old cache. */
    free(cache);
    cache = new_cache;
    cache_size = n + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
