/*
 * A329488  a(n) = A001350(n)^4.
 * https://oeis.org/A329488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A001350(n)^4
 * where A001350(n) = Lucas(n) for odd n, and Lucas(n)-2 for even n.
 * Lucas numbers are defined by: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2).
 */
#include <stdint.h>

/* fast doubling Fibonacci routine:
 * given n, computes (F(n), F(n+1)) into *fn and *fn1.
 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1)   */
    seq_t two_b = b + b;                /* 2*F(k+1) */

    seq_t c = a * (two_b - a);          /* F(2k)   = F(k)*(2F(k+1)-F(k)) */
    seq_t d = a * a + b * b;            /* F(2k+1) = F(k)^2 + F(k+1)^2   */

    if (n & 1) {                        /* n is odd  */
        *fn  = d;
        *fn1 = c + d;
    } else {                            /* n is even */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);            /* fn  = F(n)
                                         fn1 = F(n+1) */

    /* L(n) = 2*F(n+1) - F(n) */
    seq_t l = (fn1 + fn1) - fn;

    /* A001350(n) = L(n) if n odd, else L(n)-2 */
    if ((n & 1) == 0)
        l -= (seq_t)2;

    /* a(n) = l^4  = (l^2)^2 */
    seq_t sq = l * l;                  /* l^2 */
    return sq * sq;                    /* l^4 */
}
