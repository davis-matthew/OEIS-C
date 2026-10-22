/*
 * A313774  Coordination sequence Gal.6.622.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) for n > 10, with initial values a(0)=1, a(1)=5, a(2)=10,
 *            a(3)=15, a(4)=21, a(5)=25, a(6)=31, a(7)=37, a(8)=41, a(9)=47,
 *            a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_N 20000  /* coverage for typical use (n up to ~10k) */

static seq_t cache[MAX_N + 1];
static idx_t cache_len = 0;   /* number of elements already computed (index+1) */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise the first 11 terms (only once) */
    if (cache_len == 0)
    {
        seq_t init[11] = {1,5,10,15,21,25,31,37,41,47,52};
        for (idx_t i = 0; i < 11 && i <= MAX_N; ++i)
            cache[i] = init[i];
        cache_len = (cache_len < 11) ? 11 : cache_len;
        if (n < 11)
            return cache[n];
    }

    /* grow the cache up to n (note: safe for n <= MAX_N) */
    while (cache_len <= n)
    {
        idx_t i = cache_len;
        seq_t term = cache[i-1] + cache[i-3] - cache[i-4] - cache[i-6]
                     + cache[i-7] + cache[i-9] - cache[i-10];
        cache[i] = term;
        ++cache_len;
    }

    return cache[n];
}
