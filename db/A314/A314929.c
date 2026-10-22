/*
 * A314929  Coordination sequence Gal.5.189.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            -a(n-2)+a(n-3)-a(n-4)+a(n-5)+2*a(n-7)+a(n-9)-a(n-10)+a(n-11)-a(n-12)-a(n-14),
 *            for n > 14 a(0) = 1, a(1) = 5, a(2) = 9, a(3) = 14, a(4) = 20,
 *            a(5) = 21, a(6) = 30, a(7) = 33, a(8) = 35, a(9) = 44, a(10) =
 *            46, a(11) = 51, a(12) = 58, a(13) = 61, a(14) = 63
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy, memoized implementation using static cache.
       The recurrence uses both positive and negative coefficients.
       Calculations are performed in (unsigned) arithmetic which wraps
       modulo 2^bits of seq_t.  Because the final result is guaranteed
       to be in [0, 2^bits-1], the wrap does not affect correctness. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of precomputed terms */

    /* initial values for n = 0..14 */
    if (cache == NULL) {
        cache_size = 15;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;  /* fallback on out‑of‑memory */
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 9;
        cache[3] = 14;
        cache[4] = 20;
        cache[5] = 21;
        cache[6] = 30;
        cache[7] = 33;
        cache[8] = 35;
        cache[9] = 44;
        cache[10] = 46;
        cache[11] = 51;
        cache[12] = 58;
        cache[13] = 61;
        cache[14] = 63;
    }

    if (n < (idx_t)cache_size)
        return cache[(size_t)n];

    /* grow cache to accommodate n */
    size_t old_size = cache_size;
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;  /* fallback on out‑of‑memory */
    cache = tmp;
    cache_size = new_size;

    /* compute terms from old_size up to n */
    for (size_t i = old_size; i <= n; ++i) {
        if (i <= 14)  /* already initialised */
            continue;

        seq_t res = cache[i - 3];
        res += cache[i - 5];
        seq_t two = cache[i - 7];
        two += two;            /* 2 * cache[i-7] */
        res += two;
        res += cache[i - 9];
        res += cache[i - 11];

        res -= cache[i - 2];
        res -= cache[i - 4];
        res -= cache[i - 10];
        res -= cache[i - 12];
        res -= cache[i - 14];

        cache[i] = res;
    }

    return cache[(size_t)n];
}
