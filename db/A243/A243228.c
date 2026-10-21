/*
 * A243228  Number of isoscent sequences of length n with exactly two ascents.
 * https://oeis.org/A243228
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A243228
   Recurrence:
   (3*n^3 - 43*n^2 + 120*n + 20) * a(n) =
        (21*n^3 - 289*n^2 + 712*n + 400) * a(n-1)
      - (51*n^3 - 665*n^2 + 1374*n + 1540) * a(n-2)
      + 4*(12*n^3 - 145*n^2 + 230*n + 435) * a(n-3)
      - (9*n^3 - 87*n^2 + 26*n + 280)  * a(n-4)
      - 2*(3*n^3 - 34*n^2 + 43*n + 100) * a(n-5)
   Initial values: a(4)=3, a(5)=25, a(6)=128, a(7)=525, a(8)=1901
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;
    static idx_t  computed_max = 0;
    static int    initialized  = 0;

    /* initial values a(4)..a(8) */
    static const seq_t init_vals[5] = { 3ULL, 25ULL, 128ULL, 525ULL, 1901ULL };

    /* return immediately if outside offset – behaviour undefined, just 0 */
    if (n < 4) {
        return 0;
    }

    /* ensure storage for indices up to n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 11;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;             /* allocation failure */
        /* zero‑fill new elements if any */
        if (cache_cap < new_cap)
            for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }

    /* first initialisation */
    if (!initialized) {
        if (cache_cap < 11) {                 /* need indices 0..10 at least */
            seq_t *tmp = (seq_t *)realloc(cache, 11 * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = 11;
        }
        /* store known values */
        for (idx_t k = 4; k <= 8; ++k)
            cache[k] = init_vals[k - 4];
        computed_max = 8;
        initialized = 1;
    }

    if (n <= computed_max)
        return cache[n];

    /* compute new values incrementally up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* coefficients as 64‑bit signed integers */
        long long nn   = (long long)i;
        long long n2   = nn * nn;
        long long n3   = n2 * nn;

        long long coeffA =  3 * n3 - 43 * n2 + 120 * nn + 20;
        long long wt1    = 21 * n3 - 289 * n2 + 712 * nn + 400;
        long long wt2    = 51 * n3 - 665 * n2 + 1374 * nn + 1540;
        long long wt3    = 12 * n3 - 145 * n2 + 230 * nn + 435;
        long long wt4    =  9 * n3 - 87 * n2 + 26 * nn + 280;
        long long wt5    =  3 * n3 - 34 * n2 + 43 * nn + 100;

        int fast = (sizeof(seq_t) <= 8);  /* use 128‑bit intermediate for small types */

        if (fast) {
            __int128 t1 = (__int128)cache[i-1] * (__int128)wt1;
            __int128 t2 = (__int128)cache[i-2] * (__int128)wt2;
            __int128 t3 = (__int128)cache[i-3] * (__int128)(4 * wt3);
            __int128 t4 = (__int128)cache[i-4] * (__int128)wt4;
            __int128 t5 = (__int128)cache[i-5] * (__int128)(2 * wt5);

            __int128 num = t1 - t2 + t3 - t4 - t5;
            cache[i] = (seq_t)(num / (__int128)coeffA);
        } else {
            /* Big‑integer branch – all operations use seq_t */
            seq_t t1 = cache[i-1] * (seq_t)wt1;
            seq_t t2 = cache[i-2] * (seq_t)wt2;
            seq_t t3 = cache[i-3] * (seq_t)(4 * wt3);
            seq_t t4 = cache[i-4] * (seq_t)wt4;
            seq_t t5 = cache[i-5] * (seq_t)(2 * wt5);

            seq_t pos = t1 + t3;
            seq_t neg = t2 + t4 + t5;
            seq_t num = pos - neg;  /* guaranteed to be non‑negative */
            cache[i] = num / (seq_t)coeffA;
        }
    }

    computed_max = n;
    return cache[n];
}
