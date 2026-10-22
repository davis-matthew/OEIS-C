/*
 * A311533  Coordination sequence Gal.4.51.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 4, a(2) = 8, a(3) = 12, a(4) = 17, a(5) = 22,
 *            a(6) = 27, a(7) = 32, a(8) = 36, a(9) = 38, a(10) = 41, a(11) =
 *            46, a(12) = 52, a(13) = 58, a(14) = 63, a(15) = 68, a(16) = 69,
 *            a(17) = 71, a(18) = 75, a(19) = 81, a(20) = 89. For n > 20: a(n)
 *            = - a(n-2) - a(n-4) - a(n-6) + 2*a(n-7) +...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Initial values a(0) .. a(20). */
static const seq_t init_vals[21] = {
    1ull, 4ull, 8ull, 12ull, 17ull, 22ull, 27ull, 32ull, 36ull, 38ull,
    41ull, 46ull, 52ull, 58ull, 63ull, 68ull, 69ull, 71ull, 75ull, 81ull,
    89ull
};

seq_t a(idx_t n)
{
    /* Handle early indices directly. */
    if (n < 21u)
        return init_vals[(size_t)n];

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;        /* pointer to allocated elements */
    static size_t cache_len = 0;       /* number of elements stored  (>= 21) */

    /* Initialise cache with the first 21 terms on the first call. */
    if (cache == NULL) {
        cache_len = 21;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (cache == NULL)         /* allocation failed – caller will ignore */
            return 0;
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    /* If we already have the requested value, return it. */
    size_t sn = (size_t)n;
    if (sn < cache_len)
        return cache[sn];

    /* Expand cache up to n (inclusive). */
    size_t new_len = sn + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)                /* out of memory; caller will ignore */
        return 0;
    cache = tmp;

    /* Compute the missing entries sequentially. */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* i >= 21 here */
        seq_t sum = 0;

        /* Add 2 * a(i-7), a(i-9), a(i-11), a(i-13). */
        seq_t t;
        t = cache[i - 7];  t += t; sum += t;
        t = cache[i - 9];  t += t; sum += t;
        t = cache[i - 11]; t += t; sum += t;
        t = cache[i - 13]; t += t; sum += t;

        /* Subtract a(i-2), a(i-4), a(i-6), a(i-14), a(i-16), a(i-18), a(i-20). */
        sum -= cache[i - 2];
        sum -= cache[i - 4];
        sum -= cache[i - 6];
        sum -= cache[i - 14];
        sum -= cache[i - 16];
        sum -= cache[i - 18];
        sum -= cache[i - 20];

        cache[i] = sum;
    }

    cache_len = new_len;
    return cache[sn];
}
