/*
 * A382379  Length of the long leg in the unique primitive Pythagorean triple
 *          (x,y,z) such that (x-y+z)/2 is A000032(n) and such that its long
 *          leg and its hypotenuse are consecutive natural numbers.
 * https://oeis.org/A382379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + a(n-5) - 5*a(n-3) - a(n-4) * with
 *            initial values * a(0)=4, a(1)=0, a(2)=12, a(3)=24, a(4)=84 *
 *            This linear recurrence follows from the generating function *
 *            4*(1-3x+2x^2+2x^3+x^4) / ((1+x)(1-3x+x^2)(1-x-x^2))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 5‑term recurrence – keep a small cache of computed values. */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t cache_size = 0;     /* number of terms stored */
    static idx_t cache_cap   = 0;     /* allocated capacity */

    /* base values for n = 0 … 4 */
    static const seq_t base[5] = { (seq_t)4, (seq_t)0, (seq_t)12,
                                   (seq_t)24, (seq_t)84 };

    /* handle trivial cases immediately */
    if (n < 5) {
        return base[(size_t)n];
    }

    /* ensure enough room for the required index */
    idx_t need = n + 1;                 /* we need cache[0..n] */
    if (cache_cap < need) {
        idx_t new_cap = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {               /* allocation failure – return zero */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to index n */
    if (cache_size < need) {
        /* Initialize base values if cache was just allocated */
        if (cache_size == 0) {
            for (idx_t i = 0; i < 5; ++i) {
                cache[i] = base[i];
            }
            cache_size = 5;
        }
        /* recurrence for i = cache_size … n */
        for (idx_t i = cache_size; i < need; ++i) {
            /* a(i) = 3*a(i-1) + a(i-2) + a(i-5) - 5*a(i-3) - a(i-4) */
            seq_t term = 3 * cache[i-1];                 /* 3*a(i-1) */
            term += cache[i-2];                         /* + a(i-2) */
            term += cache[i-5];                         /* + a(i-5) */
            term -= 5 * cache[i-3];                     /* - 5*a(i-3) */
            term -= cache[i-4];                         /* - a(i-4) */
            cache[i] = term;
        }
        cache_size = need;
    }

    return cache[n];
}
