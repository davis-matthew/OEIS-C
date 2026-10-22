/*
 * A317567  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 6, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2) + 6*a(n-3) - 38*a(n-4) + 19*a(n-5) -
 *            7*a(n-6) + 12*a(n-7) - 19*a(n-8) - 18*a(n-9) - 10*a(n-10) -
 *            4*a(n-11) for n>12, with initial values a(1)=4, a(2)=30,
 *            a(3)=170, a(4)=997, a(5)=5837, a(6)=34152, a(7)=199987,
 *            a(8)=1170900, a(9)=6855565, a(10)=40139818, a(11)=235019013,...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values stored as seq_t.  The constants are cast from
       unsigned long long to ensure the literal is large enough. */
    static const seq_t init[13] = {
        (seq_t)0ULL,             /* dummy for index 0 */
        (seq_t)4ULL, (seq_t)30ULL, (seq_t)170ULL, (seq_t)997ULL,
        (seq_t)5837ULL, (seq_t)34152ULL, (seq_t)199987ULL,
        (seq_t)1170900ULL, (seq_t)6855565ULL, (seq_t)40139818ULL,
        (seq_t)235019013ULL, (seq_t)1376041770ULL
    };

    /* Cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static idx_t cache_last = 0;   /* highest n that has been computed */

    /* Handle n in the initial range. */
    if (n <= 12) {
        /* Allocate and initialize cache once if necessary. */
        if (cache == NULL || cache_cap <= 12) {
            size_t new_cap = 13;                 /* indices 0..12 */
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;     /* allocation failure */
            for (size_t i = 1; i <= 12; ++i)
                new_cache[i] = init[i];
            cache = new_cache;
            cache_cap = new_cap;
            cache_last = 12;
        }
        return cache[(size_t)n];
    }

    /* For n > 12: ensure cache is large enough. */
    if (cache == NULL || cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;         /* indices 0..n */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0;       /* allocation failure */
            for (size_t i = 1; i <= 12; ++i)
                cache[i] = init[i];
            cache_last = 12;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;         /* allocation failure */
            cache = tmp;
        }
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t res = (seq_t)0;
        res += cache[i-1] * (seq_t)5;
        res += cache[i-2] * (seq_t)5;
        res += cache[i-3] * (seq_t)6;
        res -= cache[i-4] * (seq_t)38;
        res += cache[i-5] * (seq_t)19;
        res -= cache[i-6] * (seq_t)7;
        res += cache[i-7] * (seq_t)12;
        res -= cache[i-8] * (seq_t)19;
        res -= cache[i-9] * (seq_t)18;
        res -= cache[i-10] * (seq_t)10;
        res -= cache[i-11] * (seq_t)4;
        cache[i] = res;
    }
    cache_last = n;
    return cache[(size_t)n];
}
