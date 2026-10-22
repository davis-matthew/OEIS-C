/*
 * A313965  Coordination sequence Gal.5.284.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = - a(n-1) - a(n-3) - a(n-4) + 2*a(n-5) + 2*a(n-6) * +
 *            2*a(n-8) + 2*a(n-9) - a(n-10) - a(n-11) - a(n-13) - a(n-14) *
 *            for n > 14, with initial values * a(0)=1, a(1)=5, a(2)=10,
 *            a(3)=17, a(4)=21, a(5)=27, * a(6)=32, a(7)=36, a(8)=46, a(9)=44,
 *            a(10)=57, * a(11)=55, a(12)=67, a(13)=70, a(14)=73
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* dynamically grows as needed   */
static idx_t cache_size = 0;         /* total allocated slots         */
static idx_t computed_upto = (idx_t)-1;   /* last index for which a(n) is stored */

/* Helper to resize cache to at least (n+1) slots (called internally). */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = n + 1;
    if (new_size < 64)            /* small guard for first allocation */
        new_size = 64;
    while (new_size <= n)
        new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    cache = new_cache;
    cache_size = new_size;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initial values for n = 0 .. 14 (constant time). */
    static const idx_t init_vals[15] = {
        1, 5, 10, 17, 21, 27, 32, 36, 46, 44,
        57, 55, 67, 70, 73
    };

    /* First call: initialise cache with the first 15 terms. */
    if (computed_upto == (idx_t)-1) {
        ensure_cache_size(15);
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = (seq_t)init_vals[i];
        computed_upto = 14;
    }

    /* If requested term is already computed, return it directly. */
    if (n <= computed_upto)
        return cache[n];

    /* Compute terms iteratively up to n (O(n)). */
    ensure_cache_size(n);
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* Use signed 64‑bit arithmetic for the recurrence intermediates. */
        long long t = 0;
        t -= (long long)cache[i-1];
        t -= (long long)cache[i-3];
        t -= (long long)cache[i-4];
        t += 2LL * ((long long)cache[i-5] + (long long)cache[i-6] +
                    (long long)cache[i-8] + (long long)cache[i-9]);
        t -= (long long)cache[i-10];
        t -= (long long)cache[i-11];
        t -= (long long)cache[i-13];
        t -= (long long)cache[i-14];
        cache[i] = (seq_t)t;
    }
    computed_upto = n;
    return cache[n];
}
