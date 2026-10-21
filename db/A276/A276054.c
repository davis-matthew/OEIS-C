/*
 * A276054  Sum of the asymmetry degrees of all compositions of n with parts
 *          in {1,2,4,6,8,10,...}.
 * https://oeis.org/A276054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{partitions of n into 1 and even parts} of their
 *            asymmetry degrees. The sequence satisfies the 11‑term linear
 *            recurrence a(n) = a(n-1) + 4*a(n-2) - a(n-3) - 5*a(n-4) -
 *            3*a(n-5) - a(n-6) + 5*a(n-8) + 2*a(n-9) - 4*a(n-10) + a(n-11),
 *            with initial values a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4...
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 72, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[11] = {
    0, 0, 0, 2, 2, 8, 16, 34, 72, 146, 294
};

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of terms stored */
    static size_t cap  = 0;   /* capacity of the cache */

    /* initialise cache on first call */
    if (!cache) {
        cap = 11;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 11; ++i) cache[i] = init_terms[i];
        size = 11;
    }

    /* ensure the cache can hold up to n */
    idx_t idx = n;
    if ((size_t)idx >= size) {
        /* grow until capacity > idx */
        while (cap <= (size_t)idx) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 16;  /* safety for cap==0, should not happen */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        /* compute missing terms up to idx */
        for (size_t i = size; i <= (size_t)idx; ++i) {
            /* recurrence uses terms i-1 .. i-11 */
            seq_t val = 0;
            val += cache[i-1];                     /* a(n-1)   */
            val += cache[i-2] * 4;                 /* 4*a(n-2) */
            val -= cache[i-3];                     /* -a(n-3)  */
            val -= cache[i-4] * 5;                 /* -5*a(n-4)*/
            val -= cache[i-5] * 3;                 /* -3*a(n-5)*/
            val -= cache[i-6];                     /* -a(n-6)  */
            val += cache[i-8] * 5;                 /* +5*a(n-8)*/
            val += cache[i-9] * 2;                 /* +2*a(n-9)*/
            val -= cache[i-10] * 4;                /* -4*a(n-10)*/
            val += cache[i-11];                    /* +a(n-11) */
            cache[i] = val;
        }
        size = (size_t)idx + 1;
    }

    return cache[(size_t)n];
}
