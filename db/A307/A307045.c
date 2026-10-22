/*
 * A307045  a(n) = Sum_{k=0..floor(n/9)} (-1)^k*binomial(n,9*k).
 * https://oeis.org/A307045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 17, i32 n <= 34, i64 n <= 67, i128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307045: a(n) = Σ_{k=0..⌊n/9⌋} (-1)^k * binomial(n,9*k)
   Recurrence (n>8):
     a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
            + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8)
   Initial: a(0)=a(1)=…=a(8)=1, a(9)=0. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t  next   = 0;          /* index of next value to compute */

    if (cache == NULL) {               /* first call: initialise */
        cap = 16;
        while (cap <= 9) cap <<= 1;    /* ensure space for indices 0..9 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i <= 8; ++i) cache[i] = 1;
        cache[9] = 0;
        next = 10;                     /* all values up to 9 are ready */
    }

    /* If the requested index is already computed, return it */
    if (n < next) return cache[n];

    /* Grow the array as necessary and compute values up to n */
    for (; next <= n; ++next) {
        if ((size_t)next >= cap) {     /* need more space */
            size_t newcap = cap << 1;
            if (newcap <= next) newcap = (size_t)next + 1;
            seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free(cache);
            cache = tmp;
            cap   = newcap;
        }

        /* Compute next using the recurrence */
        cache[next] = 9   * cache[next-1]
                    - 36  * cache[next-2]
                    + 84  * cache[next-3]
                    - 126 * cache[next-4]
                    + 126 * cache[next-5]
                    - 84  * cache[next-6]
                    + 36  * cache[next-7]
                    - 9   * cache[next-8];
    }

    return cache[n];
}
