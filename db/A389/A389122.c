/*
 * A389122  Upper (2/3)-midsequence of A000032 (Lucas numbers) and A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A389122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((4/3) * F(n+1)) = (4*F(n+1)+2)/3 where F(k) is
 *            Fibonacci with F(0)=0, F(1)=1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_rec(n >> 1, &a, &b);          /* a=F(k), b=F(k+1)   */
    seq_t t = b + b;                      /* 2*F(k+1)            */
    seq_t u = t - a;                      /* 2*F(k+1)-F(k)       */
    seq_t c = a * u;                      /* F(2k)               */
    seq_t d = a * a + b * b;              /* F(2k+1)             */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n) {
    seq_t fn, fn1;
    fib_pair_rec(n + 1, &fn, &fn1);        /* fn = F(n+1)         */
    seq_t tmp = fn * 4;                    /* 4*F(n+1)            */
    seq_t res = (tmp + 2) / 3;            /* ceil(4*F(n+1)/3)    */
    return res;
}
