/*
 * A267714  Number of nX3 0..1 arrays with every repeated value in every row
 *          and column greater than the previous repeated value.
 * https://oeis.org/A267714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+4*a(n-2)-10*a(n-3)-5*a(n-4)+20*a(n-5) *
 *            -20*a(n-7)+5*a(n-8)+10*a(n-9)-4*a(n-10)-2*a(n-11)+a(n-12) *
 *            Initial values:
 *            a(1)=6,a(2)=36,a(3)=102,a(4)=270,a(5)=546,a(6)=1080, *
 *            a(7)=1866,a(8)=3186,a(9)=5010,a(10)=7830,a(11)=11550,a(12)=17004
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 123, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t mul_small(seq_t a, unsigned int m)
{
    /* use an intermediate larger type if available to avoid overflow */
#if defined(__SIZEOF_INT128__) || defined(__UINT128_TYPE__)
    return (seq_t)((unsigned __int128)a * m);
#else
    return a * (seq_t)m;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but return 0 */

    if (n <= 12) {
        static const seq_t init[12] = {
            6, 36, 102, 270, 546, 1080,
            1866, 3186, 5010, 7830, 11550, 17004
        };
        return init[n - 1];
    }

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Initial cache allocation */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure, return 0 as fallback */
        static const seq_t init[12] = {
            6, 36, 102, 270, 546, 1080,
            1866, 3186, 5010, 7830, 11550, 17004
        };
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_len = 12;
    }

    /* Grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure: free and retry with malloc each time */
            free(cache);
            cache = NULL;
            cache_cap = 0;
            cache_len = 0;
            return a(n); /* recursive rebuild */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t val = 0;
        val += mul_small(cache[i - 1], 2);       /* a(n-1)  */
        val += mul_small(cache[i - 2], 4);       /* a(n-2)  */
        val -= mul_small(cache[i - 3], 10);      /* a(n-3)  */
        val -= mul_small(cache[i - 4], 5);       /* a(n-4)  */
        val += mul_small(cache[i - 5], 20);      /* a(n-5)  */
        val -= mul_small(cache[i - 7], 20);      /* a(n-7)  */
        val += mul_small(cache[i - 8], 5);       /* a(n-8)  */
        val += mul_small(cache[i - 9], 10);      /* a(n-9)  */
        val -= mul_small(cache[i -10], 4);       /* a(n-10) */
        val -= mul_small(cache[i -11], 2);       /* a(n-11) */
        val += cache[i -12];                    /* a(n-12) */
        cache[i] = val;
    }
    cache_len = n;

    return cache[n - 1];
}
