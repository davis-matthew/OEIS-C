/*
 * A242300  a(n) = Sum_{0<=i<j<=n}L(i)*L(j), where L(k)=A000032(k) is the k-th
 *          Lucas number.
 * https://oeis.org/A242300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (L(n+1)-1)^2 + L(n-1) + (5*(-1)^n-3)/2, where L(k) is the
 *            k‑th Lucas number. Largest argument is computed by fast doubling
 *            of Fibonacci numbers. For n = 0 the value is 0
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast doubling method for Fibonacci numbers:
   for given n, it computes F(n) and F(n+1) and stores them in *fn and *fn1. */
static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;     /* F(0)   */
    seq_t b = 1;     /* F(1)   */
    /* find highest set bit */
    idx_t top = 63;
    while (top > 0 && ((n >> top) & 1ULL) == 0)
        --top;
    for (idx_t i = top; i <= 63; --i) { /* iterate from MSB to LSB */
        /* c = F(2k)   = a * (2*b - a)   */
        seq_t two_b = b + b;
        seq_t temp  = two_b - a;          /* 2*b - a  (always non‑negative)  */
        seq_t c     = a * temp;
        /* d = F(2k+1) = a*a + b*b      */
        seq_t d     = a * a + b * b;
        if (((n >> i) & 1ULL) == 0) {
            a = c;
            b = d;
        } else {
            a = d;
            b = c + d;
        }
        /* stop if i was the last bit processed */
        if (i == 0) break;
    }
    *fn  = a;
    *fn1 = b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t Fn, Fn1;
    fib_pair(n, &Fn, &Fn1);            /* F(n), F(n+1) */

    /* L(n-1) = 3*F(n) - F(n+1)   (valid for n>=1) */
    seq_t Ln_minus1 = 3 * Fn - Fn1;

    /* Compute L(n+1):
       L(k) = 2*F(k+1) - F(k)   -->  L(n+1) = 2*F(n+2) - F(n+1) */
    seq_t Fn2, Fn3;
    fib_pair(n + 1, &Fn2, &Fn3);    /* F(n+1), F(n+2) */
    seq_t Ln_plus1 = 2 * Fn3 - Fn2;

    /* Compute (L(n+1)-1)^2 + L(n-1) */
    seq_t termA = Ln_plus1 - 1;
    seq_t square = termA * termA;
    seq_t res = square + Ln_minus1;

    /* Add the small correction term */
    if ((n & 1ULL) == 0)
        res += 1;
    else
        res -= 4;   /* unsigned subtraction; the result is always positive */

    return res;
}
