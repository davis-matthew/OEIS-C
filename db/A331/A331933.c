/*
 * A331933  Number of semi-lone-child-avoiding rooted trees with at most one
 *          distinct non-leaf branch directly under any vertex.
 * https://oeis.org/A331933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=2..n-2} floor((n-1)/i) * a(i) Initial values:
 *            a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle the tiny cases directly */
    if (n <= 3)
        return (seq_t)1;

    /* Static memoization cache */
    static seq_t *cache = NULL;   /* array indexed by n (1‑based) */
    static size_t cap   = 0;      /* highest index currently valid in cache */
    static idx_t  last  = 0;      /* largest n already computed */

    /* Grow the cache if necessary */
    if ((idx_t)n > (idx_t)cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap < (size_t)n)
            newcap <<= 1;                /* double until sufficient */

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (newcap + 1));
        if (!tmp)
            abort();                     /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing values from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = (seq_t)1;            /* the "+ 1" of the recurrence */

        if (i >= 4) {                    /* non‑empty sum only then */
            for (idx_t j = 2; j <= i - 2; ++j) {
                unsigned int coeff = (unsigned int)((i - 1) / j);
                val += mul_small(cache[j], coeff);
            }
        }

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
