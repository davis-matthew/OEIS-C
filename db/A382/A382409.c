/*
 * A382409  Semiperimeter of the unique primitive Pythagorean triple (a,b,c)
 *          such that (a-b+c)/2 is A000032(n) and such that its long leg and
 *          its hypotenuse are consecutive natural numbers.
 * https://oeis.org/A382409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) * (2 * Lucas(n) - 1) Lucas numbers satisfy
 *            L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2). The implementation uses the
 *            fast doubling method for Fibonacci numbers (F(0)=0, F(1)=1) and
 *            the identity L(n)=2*F(n+1)-F(n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the pair (F(n), F(n+1)) using fast doubling.
   The function writes the results through the supplied pointers. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    idx_t half = n >> 1;
    fib_pair(half, fn, fn1);

    seq_t a = *fn;   /* F(k)   */
    seq_t b = *fn1;  /* F(k+1) */

    /* c = F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    seq_t two_b_minus_a = b + b;      /* 2*F(k+1) */
    two_b_minus_a -= a;              /* 2*F(k+1)-F(k) */
    seq_t c = a * two_b_minus_a;     /* F(2k) */

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t d = a * a;                 /* F(k)^2 */
    d += b * b;                      /* + F(k+1)^2 */

    if ((n & 1) == 0) {              /* even n */
        *fn  = c;
        *fn1 = d;
    } else {                         /* odd n  */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) and Fibonacci(n+1) */
    seq_t f_n, f_np1;
    fib_pair(n, &f_n, &f_np1);

    /* Lucas(n) = 2*F(n+1) - F(n) */
    seq_t two_f_np1 = f_np1 + f_np1;   /* 2*F(n+1) */
    seq_t L = two_f_np1 - f_n;         /* Lucas */

    /* a(n) = L * (2*L - 1) */
    seq_t twoL = L + L;                /* 2*L */
    seq_t twoL_minus1 = twoL;
    twoL_minus1 -= 1;                 /* 2*L - 1 */

    return L * twoL_minus1;
}
