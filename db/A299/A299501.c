/*
 * A299501  Expansion of (1 - 6*x + 7*x^2 - 2*x^3 + x^4)^(-1/2).
 * https://oeis.org/A299501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A299501: Expansion of (1 - 6*x + 7*x^2 - 2*x^3 + x^4)^(-1/2)
 * Recurrence: (2 - n)*a(n-4) + (2n-3)*a(n-3) + (7-7n)*a(n-2) + (6n-3)*a(n-1) = n*a(n)
 * Initial values: a(0)=1, a(1)=3, a(2)=10, a(3)=37
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t base_vals[4] = { (seq_t)1, (seq_t)3, (seq_t)10, (seq_t)37 };

seq_t a(idx_t n)
{
    /*--- static cache for memoisation --------------------------------*/
    static seq_t *cache   = NULL;   /* array holding computed terms   */
    static idx_t  cap     = 0;      /* capacity of the array          */
    static idx_t  size    = 0;      /* number of terms already computed */

    if (n < size)            /* term already known, return it    */
        return cache[n];

    /*--- grow the cache if necessary --------------------------------*/
    if (n + 1 > cap) {
        idx_t newcap = n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /*--- initialise the base terms if they are not yet set ---------*/
    if (size == 0) {
        cache[0] = base_vals[0];
        if (cap > 1) cache[1] = base_vals[1];
        if (cap > 2) cache[2] = base_vals[2];
        if (cap > 3) cache[3] = base_vals[3];
        size = (cap < 4 ? cap : 4);
    }

    /*--- for an incomplete initialisation (size < 4) ---------------*/
    if (size < 4) {
        for (idx_t i = size; i < 4 && i < cap; ++i)
            cache[i] = base_vals[i];
        size = (cap < 4 ? cap : 4);
    }

    /*--- compute missing terms up to index n ----------------------*/
    for (idx_t i = size; i <= n; ++i) {
        if (i < 4)                     /* base cases already set     */
            continue;

        /* use unsigned 128‑bit intermediate to avoid overflow
           when seq_t is 64‑bit. For arbitrary‑precision seq_t,
           the cast to unsigned __int128 works because the value
           fits into 128 bits in the required range.              */
        unsigned __int128 pos1 = (unsigned __int128)(2 * i - 3) * (unsigned __int128)cache[i - 3];
        unsigned __int128 pos2 = (unsigned __int128)(6 * i - 3) * (unsigned __int128)cache[i - 1];
        unsigned __int128 neg1 = (unsigned __int128)(i - 2)      * (unsigned __int128)cache[i - 4];
        unsigned __int128 neg2 = (unsigned __int128)(7 * i - 7)  * (unsigned __int128)cache[i - 2];

        unsigned __int128 pos  = pos1 + pos2;          /* (2n-3)*a(n-3) + (6n-3)*a(n-1) */
        unsigned __int128 neg  = neg1 + neg2;          /* (n-2)*a(n-4) + (7n-7)*a(n-2)  */
        unsigned __int128 numer = pos - neg;           /* numerator of the recurrence   */

        cache[i] = (seq_t)(numer / (unsigned __int128)i);
    }

    size = n + 1;           /* we now know all terms up to n */
    return cache[n];
}
