/*
 * A315469  Coordination sequence Gal.4.134.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - a(n-8) (n > 8) Initial values: a(0)=1,
 *            a(1)=6, a(2)=11, a(3)=16, a(4)=21, a(5)=26, a(6)=31, a(7)=36,
 *            a(8)=42
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Constant values for indices 0..8 */
static const seq_t base[9] = {1,6,11,16,21,26,31,36,42};

/* Memoization cache */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* how many slots currently allocated */
static idx_t max_computed = 0;/* largest n that has been computed and stored in cache */

/* Helper to ensure the cache is large enough to hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap == 0) {
        /* first use: allocate room for at least indices 0..8 */
        cache_cap = (n < 9) ? 9 : (n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 9; ++i) {
            cache[i] = base[i];
        }
        max_computed = 8;
    } else if (n >= cache_cap) {
        /* grow the cache to accommodate new indices */
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n <= 8) {
        return base[n];
    }

    ensure_cache(n);

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term =
                2 * cache[i-1] - 2 * cache[i-2] +
                2 * cache[i-3] - 2 * cache[i-4] +
                2 * cache[i-5] - 2 * cache[i-6] +
                2 * cache[i-7] -     cache[i-8];
            cache[i] = term;
        }
        max_computed = n;
    }

    return cache[n];
}
