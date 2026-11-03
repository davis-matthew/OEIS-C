/* fib_pair: the Fibonacci pair (F(n), F(n+1)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_FIB_PAIR_H
#define OEISC_UTILS_FIB_PAIR_H
#include <stdint.h>

/* Fast doubling: computes (F_n, F_{n+1}) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);               /* a = F_k,  b = F_{k+1}   */

    seq_t two_b = b << 1;                   /* 2*F_{k+1}              */
    seq_t c = a * (two_b - a);              /* F_{2k}                  */
    seq_t d = a * a + b * b;                /* F_{2k+1}                */

    if (n & 1) {                            /* n is odd                */
        *fn  = d;                           /* F_{2k+1}                */
        *fn1 = c + d;                       /* F_{2k+2} = F_{2k} + F_{2k+1} */
    } else {                                /* n is even               */
        *fn  = c;                           /* F_{2k}                  */
        *fn1 = d;                           /* F_{2k+1}                */
    }
}

#endif
