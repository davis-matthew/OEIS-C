/*
 * A292612  a(n) = F(n)^2 + 4*(-1)^n = F(n+3)*F(n-3), where F = A000045.
 * https://oeis.org/A292612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = F(n+3)*F(n-3) where F(0)=0, F(1)=1 and
 *            F(k)=F(k-1)+F(k-2). This is equivalent to a(n) = F(n)^2 +
 *            4*(-1)^n
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 23, i64 n <= 47, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Fast doubling Fibonacci: computes F(n) and F(n+1) into *fn and *fn1.
   Works for any n >= 0. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;        /* a = F(k), b = F(k+1) where k = n>>1 */
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k) = a * (2*b - a)   */
    seq_t two = 2;
    seq_t c = a * ((b * two) - a);

    /* d = F(2k+1) = a*a + b*b   */
    seq_t d = a * a + b * b;

    if ((n & 1ULL) == 0) {
        *fn  = c;      /* F(n)   = F(2k)   */
        *fn1 = d;      /* F(n+1) = F(2k+1) */
    } else {
        *fn  = d;                 /* F(n)   = F(2k+1) */
        *fn1 = c + d;             /* F(n+1) = F(2k+2) */
    }
}

/* Public function that returns the n-th term of A292612. */
seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 0ULL) return (seq_t)4;
    if (n == 1ULL) return (seq_t)-3;
    if (n == 2ULL) return (seq_t)5;

    /* For n >= 3 compute F(n-3) and F(n+3) */
    seq_t f1, tmp;          /* f1 = F(n-3)        */
    fib_pair(n - 3, &f1, &tmp);

    seq_t f3, dummy;        /* f3 = F(n+3)        */
    fib_pair(n + 3, &f3, &dummy);

    return f3 * f1;
}
