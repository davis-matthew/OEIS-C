/*
 * A257940  y-values in the solutions to x^2 + x = 5*y^2 + y.
 * https://oeis.org/A257940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 322*a(n-2) - a(n-4) + 32 for n ≥ 5 with initial values:
 *            a(1)=0, a(2)=1, a(3)=52, a(4)=357, a(5)=16776
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Reject or handle non‑positive indices */
    if (n < 1)
        return (seq_t)0;

    /* Return the five known initial terms directly */
    if (n <= 5) {
        static const seq_t init[5] = { (seq_t)0, (seq_t)1, (seq_t)52,
                                      (seq_t)357, (seq_t)16776 };
        return init[n - 1];
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;          /* a(1) .. a(len) stored */
    static size_t len   = 0;             /* number of terms already computed */
    static size_t cap   = 0;             /* allocated capacity of cache */

    /* Ensure enough space for terms up to n */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        /* realloc returns void*, cast it to seq_t* */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* If we haven't stored the first five terms yet, initialise them */
    if (len < 5) {
        static const seq_t init[5] = { (seq_t)0, (seq_t)1, (seq_t)52,
                                      (seq_t)357, (seq_t)16776 };
        size_t upto = 5 < len ? len : 5;
        for (size_t i = len; i < upto; ++i)
            cache[i] = init[i];
        len = upto;
    }

    /* Compute missing terms up to and including n */
    for (size_t i = len; i < (size_t)n; ++i) {
        seq_t val;
        if (i < 5) {                     /* i is 0‑based – a(1) is i=0 */
            static const seq_t init[5] = { (seq_t)0, (seq_t)1, (seq_t)52,
                                            (seq_t)357, (seq_t)16776 };
            val = init[i];
        } else {                         /* recurrence for n ≥ 6  */
            val = (seq_t)322 * cache[i - 2] - cache[i - 4] + (seq_t)32;
        }
        cache[i] = val;
    }
    len = (size_t)n;

    return cache[n - 1];
}
