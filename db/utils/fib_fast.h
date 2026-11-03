/* fib_fast: the Fibonacci pair (F(n), F(n+1)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_FIB_FAST_H
#define OEISC_UTILS_FIB_FAST_H
#include <stdint.h>

/* Fast doubling Fibonacci: for a given n returns F(n) in *f
   and F(n+1) in *f_next.  Works for any seq_t that supports
   integer arithmetic. */
static void fib_fast(idx_t n, seq_t *f, seq_t *f_next) {
    if (n == 0) {
        *f = (seq_t)0;
        *f_next = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_fast(n >> 1, &a, &b);          /* a = F(k), b = F(k+1)  (k = n/2) */

    /* Compute F(2k) and F(2k+1) */
    seq_t two_b = b + b;                /* 2 * F(k+1) */
    seq_t f_2k     = a * (two_b - a);   /* a * (2*b - a)   */
    seq_t f_2k1    = a * a + b * b;     /* a^2 + b^2       */

    if (n & 1) {
        *f = f_2k1;                    /* F(2k+1) = F(n)   */
        *f_next = f_2k + f_2k1;        /* F(2k+2) = F(n+1) */
    } else {
        *f = f_2k;                     /* F(2k) = F(n)     */
        *f_next = f_2k1;               /* F(2k+1) = F(n+1) */
    }
}

#endif
