/*
 * A275268  Number of 4 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 7*a(n-2) - 9*a(n-3) - 8*a(n-4) - 12*a(n-5) +
 *            16*a(n-6) for n > 7, with initial values a(1)=14, a(2)=64,
 *            a(3)=396, a(4)=2564, a(5)=17036, a(6)=114184, a(7)=767400
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t base_vals[7] = {
    (seq_t)14,          /* a(1) */
    (seq_t)64,          /* a(2) */
    (seq_t)396,         /* a(3) */
    (seq_t)2564,        /* a(4) */
    (seq_t)17036,       /* a(5) */
    (seq_t)114184,      /* a(6) */
    (seq_t)767400       /* a(7) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* stores a(1) .. a(max_computed) */
    static idx_t cache_size = 0;    /* allocated capacity (in terms of count) */
    static idx_t max_computed = 0;  /* highest n already computed */

    if (n <= 7)
        return base_vals[n - 1];

    /* Grow cache if necessary */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) { /* allocation failure – behavior unspecified */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = n;
    }

    /* Initialise base values once */
    if (max_computed < 7) {
        for (idx_t i = 0; i < 7 && i < cache_size; ++i)
            cache[i] = base_vals[i];
        max_computed = 7;
    }

    /* Compute remaining terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = mul_small(cache[i - 2], 8);        /* 8*a(i-1) */
        seq_t t2 = mul_small(cache[i - 7], 16);       /* 16*a(i-6) */
        seq_t sum = mul_small(cache[i - 3], 7);       /* 7*a(i-2) */
        sum += mul_small(cache[i - 4], 9);            /* 9*a(i-3) */
        sum += mul_small(cache[i - 5], 8);            /* 8*a(i-4) */
        sum += mul_small(cache[i - 6], 12);           /* 12*a(i-5) */

        seq_t s = t1;
        s += t2;                                      /* s = 8*a(i-1) + 16*a(i-6) */
        cache[i - 1] = s;                              /* store a(i) after subtraction */
        cache[i - 1] -= sum;
    }

    max_computed = n;
    return cache[n - 1];
}
