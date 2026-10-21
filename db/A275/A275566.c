/*
 * A275566  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,0) (-1,2) or (0,-2) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 7*a(n-5) - 5*a(n-6) +
 *            2*a(n-8) - a(n-9) for n>10 Initial values: a(1)=3, a(2)=54,
 *            a(3)=126, a(4)=294, a(5)=672, a(6)=1536, a(7)=3552, a(8)=8214,
 *            a(9)=19092, a(10)=44376 Computation uses memoisation; sequence
 *            defined for n>=1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 49, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index cached (>=0) */

    /* Base values (index 0 unused) */
    static const seq_t base_values[11] = {
        0, /* dummy 0-index */
        (seq_t)3,    /* a(1) */
        (seq_t)54,   /* a(2) */
        (seq_t)126,  /* a(3) */
        (seq_t)294,  /* a(4) */
        (seq_t)672,  /* a(5) */
        (seq_t)1536, /* a(6) */
        (seq_t)3552, /* a(7) */
        (seq_t)8214, /* a(8) */
        (seq_t)19092,/* a(9) */
        (seq_t)44376 /* a(10) */
    };

    if (n == (idx_t)0) return (seq_t)0; /* undefined, but keeps compiler happy */

    /* Grow cache if necessary */
    if (n > cache_size) {
        /* Determine new allocation size */
        idx_t new_size = n + 1;   /* 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure – bail with 0 */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise newly allocated entries to 0 */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)0;
        }

        /* Copy base values if they belong in the new range */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = base_values[i];
            } else {
                /* Recurrence of order 9 */
                cache[i] =  (seq_t)3 * cache[i - 1]
                           - (seq_t)5 * cache[i - 3]
                           + (seq_t)1 * cache[i - 4]
                           + (seq_t)7 * cache[i - 5]
                           - (seq_t)5 * cache[i - 6]
                           + (seq_t)2 * cache[i - 8]
                           - (seq_t)1 * cache[i - 9];
            }
        }
        cache_size = n;
    }

    return cache[n];
}
