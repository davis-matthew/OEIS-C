/*
 * A267693  Decimal expansion of 477/4237.
 * https://oeis.org/A267693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A267693
   Formula:  a(n) = the (n+1)-th decimal digit of 477/4237
   (0‑based index, where a(0)=1 is the first digit after the decimal point).  */

#include <stdlib.h>
#include <stdint.h>

/* External integer types provided by the build. */
extern struct { /* placeholder for build‑defined types */ } *no_distinction;

seq_t a(idx_t n)
{
    /* The denominator of the fraction.  */
    const uint32_t D = 4237U;
    /* The numerator of the fraction.  */
    const uint32_t P = 477U;

    /* Static state preserved between calls. */
    static seq_t *cache = NULL;    /* cached digits */
    static idx_t  csize = 0;       /* current number of cached digits */
    static idx_t  cap   = 0;       /* allocated capacity of cache */
    static uint32_t rem = P;       /* current remainder before generating next digit */

    /* Expand the cache if necessary.  */
    if (n >= csize) {
        /* Grow cache to at least n+1 elements.  */
        idx_t needed = n + 1;
        if (cap < needed) {
            /* Double until it fits, but at least 16.  */
            idx_t new_cap = cap ? cap << 1 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* Generate digits up to n.  */
        for (idx_t i = csize; i <= n; ++i) {
            uint32_t prod = rem * 10U;           /* remainder multiplied by 10          */
            uint32_t d    = prod / D;           /* next digit                          */
            rem = prod % D;                     /* new remainder for next step         */
            cache[i] = (seq_t)d;                /* store the digit                     */
        }

        csize = n + 1;                         /* update cached size                  */
    }

    return cache[n];
}
