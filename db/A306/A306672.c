/*
 * A306672  Partial sums of the even Lucas numbers (A014448).
 * https://oeis.org/A306672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (L(3 n + 2) + 1)/2 (Lucas numbers)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;                      /* F(0)   */
    seq_t b = 1;                      /* F(1)   */

    if (n == 0)                       /* trivial case */
    {
        *fn  = a;
        *fn1 = b;
        return;
    }

    /* find the highest set bit of n (n > 0) */
    idx_t mask = (idx_t)1 << (63 - __builtin_clzll(n));

    for (; mask; mask >>= 1)
    {
        seq_t two_b = b + b;          /* 2*F(k+1) */
        seq_t d = a * (two_b - a);    /* F(2k)   = F(k) * [2*F(k+1) – F(k)]   */
        seq_t e = a * a + b * b;      /* F(2k+1) = F(k)^2 + F(k+1)^2          */

        if ((n & mask) == 0)          /* bit 0 : next even index  */
        {
            a = d;
            b = e;
        }
        else                          /* bit 1 : next odd index   */
        {
            a = e;
            b = d + e;
        }
    }

    *fn  = a;        /* F(n)   */
    *fn1 = b;        /* F(n+1) */
}

seq_t a(idx_t n)
{
    /* k = 3 n + 2 gives the Lucas index needed */
    idx_t k = n * (idx_t)3 + 2;

    seq_t Fk, Fk1;
    fib_pair(k, &Fk, &Fk1);                /* Fk = F(k), Fk1 = F(k+1) */

    /* Lucas number: L(k) = 2*F(k+1) – F(k) */
    seq_t Lk = Fk1 + Fk1 - Fk;

    /* a(n) = (L(3n+2)+1)/2 */
    return (Lk + 1) / (seq_t)2;
}
