/*
 * A302147  Number of nX5 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 65*a(n-2) + 88*a(n-3) + 132*a(n-4) * -
 *            362*a(n-5) - 39*a(n-6) + 438*a(n-7) - 19*a(n-8) - 208*a(n-9) * -
 *            16*a(n-10) + 26*a(n-11) + 4*a(n-12), for n > 12 * Initial
 *            values: a(1)=1, a(2)=3, a(3)=11, a(4)=56, a(5)=289, * a(6)=1529,
 *            a(7)=8152, a(8)=43676, a(9)=234707, * a(10)=1264018...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[13] = {
    0ULL,            /* base[0] unused, a(0)=0 for convenience */
    1ULL, 3ULL, 11ULL, 56ULL, 289ULL, 1529ULL,
    8152ULL, 43676ULL, 234707ULL, 1264018ULL,
    6818793ULL, 36833925ULL
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* grow capacity: double until enough */
    idx_t cap = cache_len ? cache_len : 13;
    while (cap <= n) cap <<= 1;

    if (!cache) {
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) return;
        /* initialize base values up to min(12,n) */
        for (idx_t i = 1; i <= 12 && i <= n; ++i) cache[i] = base[i];
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
        if (!tmp) return;
        cache = tmp;
    }

    /* compute new values */
    for (idx_t k = (cache_len > 0 ? cache_len + 1 : 1); k <= n; ++k) {
        if (k <= 12) {
            cache[k] = base[k];
        } else {
            seq_t pos = 0, neg = 0, t;
            t = cache[k - 1];
            pos += t * (seq_t)14;
            t = cache[k - 2];
            neg += t * (seq_t)65;
            t = cache[k - 3];
            pos += t * (seq_t)88;
            t = cache[k - 4];
            pos += t * (seq_t)132;
            t = cache[k - 5];
            neg += t * (seq_t)362;
            t = cache[k - 6];
            neg += t * (seq_t)39;
            t = cache[k - 7];
            pos += t * (seq_t)438;
            t = cache[k - 8];
            neg += t * (seq_t)19;
            t = cache[k - 9];
            neg += t * (seq_t)208;
            t = cache[k -10];
            neg += t * (seq_t)16;
            t = cache[k -11];
            pos += t * (seq_t)26;
            t = cache[k -12];
            pos += t * (seq_t)4;
            cache[k] = pos - neg;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n];
}
