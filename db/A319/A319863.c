/*
 * A319863  a(2n) = a(n) + a(n+1) + n (n > 1), a(2n+1) = a(n) + a(n-1) + 1 (n
 *          >= 1) with a(0) = a(1) = 1 and a(2) = 2.
 * https://oeis.org/A319863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A319863
 *  a(2n)   = a(n) + a(n+1) + n        (n > 1)
 *  a(2n+1) = a(n) + a(n-1) + 1        (n ≥ 1)
 *  a(0) = a(1) = 1
 *  a(2) = 2
 *
 *  The values are >0 for all n ≥ 0, so a cache entry of 0 is used
 *  to indicate that the value has not yet been computed.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */

/* Memoised computation of a(n) */
seq_t a(idx_t n)
{
    /* Static cache array: cache[i] == 0  ⇔  a(i) not known yet */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* number of allocated elements */

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – abort */
            /* The build environment never checks this path, but we
               exit cleanly if it happens. */
            abort();
        }
        /* Initialise the new part to 0 (unknown values) */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Return memoised value if present */
    if (cache[n] != 0)
        return cache[n];

    /* Compute the value recursively */
    seq_t result;
    if (n == 0) {
        result = 1;
    } else if (n == 1) {
        result = 1;
    } else if (n == 2) {
        result = 2;
    } else if ((n & 1) == 0) {               /* even: n = 2*m,   m > 1 */
        idx_t m = n >> 1;
        seq_t a_m  = a(m);
        seq_t a_m1 = a(m + 1);
        result = a_m + a_m1 + (seq_t)m;
    } else {                                 /* odd:  n = 2*m+1,  m ≥ 1 */
        idx_t m = (n - 1) >> 1;               /* m = floor(n/2)   */
        seq_t a_m   = a(m);
        seq_t a_m1  = a(m - 1);
        result = a_m + a_m1 + 1;
    }

    /* Cache the result for future calls */
    cache[n] = result;
    return result;
}
