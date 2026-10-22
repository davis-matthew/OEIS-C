/*
 * A347854  a(n) = (6*n)!/((3*n)!*(2*n)!) * (n/2)!/(3*n/2)!.
 * https://oeis.org/A347854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A347854
   Formula: a(n) = (6*n)!/((3*n)!*(2*n)!) * (n/2)!/(3*n/2)!
   Recurrence: a(0)=1, a(1)=40
   a(2*m)   = 24*(12m-1)*(12m-5)*(12m-7)*(12m-11) / ( m*(2m-1)*(3m-1)*(3m-2) ) * a(2*m-2)
   a(2*m+1) = 96*(12m+1)*(12m-1)*(12m+5)*(12m-5) / ( m*(2m+1)*(6m+1)*(6m-1) ) * a(2*m-1) */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 uint128_t;
#endif

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;

    /* first allocation */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;              /* a(0)=1 */
        cache_len = 1;
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        /* allocate enough space for indices up to n */
        while (cache_cap <= (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;     /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)40;     /* a(1)=40 */
                continue;
            }

            if ((i & 1) == 0) { /* even: i = 2*m */
                idx_t m = i >> 1;
#ifdef __SIZEOF_INT128__
                uint128_t num = (uint128_t)24;
                num *= (uint128_t)(12 * m - 1);
                num *= (uint128_t)(12 * m - 5);
                num *= (uint128_t)(12 * m - 7);
                num *= (uint128_t)(12 * m - 11);

                uint128_t den = (uint128_t)m;
                den *= (uint128_t)(2 * m - 1);
                den *= (uint128_t)(3 * m - 1);
                den *= (uint128_t)(3 * m - 2);

                uint128_t res = (uint128_t)cache[i - 2];
                res = res * num / den;
                cache[i] = (seq_t)res;
#else
                seq_t num = (seq_t)24;
                num *= (seq_t)(12 * m - 1);
                num *= (seq_t)(12 * m - 5);
                num *= (seq_t)(12 * m - 7);
                num *= (seq_t)(12 * m - 11);

                seq_t den = (seq_t)m;
                den *= (seq_t)(2 * m - 1);
                den *= (seq_t)(3 * m - 1);
                den *= (seq_t)(3 * m - 2);

                cache[i] = (cache[i - 2] * num) / den;
#endif
            } else { /* odd: i = 2*m+1 */
                idx_t m = (i - 1) >> 1;
#ifdef __SIZEOF_INT128__
                uint128_t num = (uint128_t)96;
                num *= (uint128_t)(12 * m + 1);
                num *= (uint128_t)(12 * m - 1);
                num *= (uint128_t)(12 * m + 5);
                num *= (uint128_t)(12 * m - 5);

                uint128_t den = (uint128_t)m;
                den *= (uint128_t)(2 * m + 1);
                den *= (uint128_t)(6 * m + 1);
                den *= (uint128_t)(6 * m - 1);

                uint128_t res = (uint128_t)cache[i - 2];
                res = res * num / den;
                cache[i] = (seq_t)res;
#else
                seq_t num = (seq_t)96;
                num *= (seq_t)(12 * m + 1);
                num *= (seq_t)(12 * m - 1);
                num *= (seq_t)(12 * m + 5);
                num *= (seq_t)(12 * m - 5);

                seq_t den = (seq_t)m;
                den *= (seq_t)(2 * m + 1);
                den *= (seq_t)(6 * m + 1);
                den *= (seq_t)(6 * m - 1);

                cache[i] = (cache[i - 2] * num) / den;
#endif
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
