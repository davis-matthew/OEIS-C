/*
 * A291988  Expansion of 1/((1-x)*(1-2*x^2)*(1-3*x^3)*(1-4*x^4)*(1-5*x^5)).
 * https://oeis.org/A291988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 45, u64 n <= 102, u128 n <= 221,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 2*a(n-2) + a(n-3) + a(n-4)
 *              - 5*a(n-5) - 7*a(n-6) - 14*a(n-7)
 *              + 7*a(n-8) + 19*a(n-9) + 26*a(n-10)
 *              + 10*a(n-11) + 20*a(n-12)
 *              - 60*a(n-13) - 120*a(n-14) + 120*a(n-15)
 *
 * Initial values:
 * a(0)=1, a(1)=1, a(2)=3, a(3)=6, a(4)=14, a(5)=25,
 * a(6)=50, a(7)=84, a(8)=165, a(9)=280, a(10)=503,
 * a(11)=826, a(12)=1477, a(13)=2386, a(14)=4067, a(15)=6625
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cached_last = 0;   /* last computed index in cache */

/* initial 16 terms */
static const seq_t init_vals[16] = {
    (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)14, (seq_t)25,
    (seq_t)50, (seq_t)84, (seq_t)165, (seq_t)280, (seq_t)503,
    (seq_t)826, (seq_t)1477, (seq_t)2386, (seq_t)4067, (seq_t)6625
};

seq_t a(idx_t n)
{
    /* If n is one of the first 16 terms, return it directly */
    if (n < 16) {
        return init_vals[n];
    }

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) {
            return (seq_t)0;   /* allocation failure */
        }
        for (idx_t i = 0; i < 16; ++i) {
            cache[i] = init_vals[i];
        }
        cached_last = 15;
    }

    /* Grow cache if required */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0;   /* reallocation failure */
        }
        cache = tmp;
        cache_len = new_len;
        /* new elements are not initialised yet; they will be computed later */
    }

    /* Compute missing terms using the recurrence */
    if (n > cached_last) {
        for (idx_t i = cached_last + 1; i <= n; ++i) {
            seq_t val = cache[i-1];
            val = val + cache[i-2] * (seq_t)2;
            val = val + cache[i-3];
            val = val + cache[i-4];
            val = val - cache[i-5] * (seq_t)5;
            val = val - cache[i-6] * (seq_t)7;
            val = val - cache[i-7] * (seq_t)14;
            val = val + cache[i-8] * (seq_t)7;
            val = val + cache[i-9] * (seq_t)19;
            val = val + cache[i-10] * (seq_t)26;
            val = val + cache[i-11] * (seq_t)10;
            val = val + cache[i-12] * (seq_t)20;
            val = val - cache[i-13] * (seq_t)60;
            val = val - cache[i-14] * (seq_t)120;
            val = val + cache[i-15] * (seq_t)120;
            cache[i] = val;
        }
        cached_last = n;
    }

    return cache[n];
}
