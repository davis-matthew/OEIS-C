/*
 * A318404  a(n) = F(n+1)^4 - 4*F(n-1)*F(n)^3, where F(n) = A000045(n), the
 *          n-th Fibonacci number.
 * https://oeis.org/A318404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1)^4 - 4*F(n-1)*F(n)^3 where F(n) is the Fibonacci
 *            number with F(0)=0, F(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* fast doubling algorithm – returns F(n) in *fn and F(n+1) in *fn1 */
static void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair_rec(n >> 1, &a, &b);          /* a=F(k),  b=F(k+1) with k=n/2 */
    seq_t two_b = (seq_t)2 * b;            /* 2*F(k+1)   */
    seq_t t    = two_b - a;                /* 2*F(k+1)-F(k) = F(2k+1)-F(k) */
    seq_t c    = a * t;                    /* F(2k)      */
    seq_t d    = a * a + b * b;            /* F(2k+1)    */
    if ((n & 1) == 0)
    {
        *fn  = c;
        *fn1 = d;
    }
    else
    {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair_rec(n, &fn, &fn1);          /* fn = F(n),  fn1 = F(n+1) */
    seq_t fnm1 = fn1 - fn;                /* F(n-1)   (works also for n=0) */
    seq_t fn3   = fn * fn * fn;           /* F(n)^3   */
    seq_t fn1_sq= fn1 * fn1;              /* F(n+1)^2 */
    seq_t part1 = fn1_sq * fn1_sq;        /* F(n+1)^4 */
    seq_t part2 = (seq_t)4 * fnm1 * fn3;  /* 4*F(n-1)*F(n)^3 */
    return part1 - part2;
}
