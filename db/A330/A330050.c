/*
 * A330050  a(n) = 2*((-1)^n - 1)*(F(n) - 1) - (3*(-1)^n + 7)/2 * F(n+1) +
 *          5*F(n+1)^2.
 * https://oeis.org/A330050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*((-1)^n - 1)*(F(n) - 1) - (3*(-1)^n + 7)/2 * F(n+1) +
 *            5*F(n+1)^2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Fibonacci numbers */
    seq_t fn  = fib(n);     /* F(n)   */
    seq_t fn1 = fib(n + 1); /* F(n+1) */

    /* Coefficient for the second term: (3*(-1)^n + 7)/2 */
    seq_t coeff2 = (n & 1) ? (seq_t)2 : (seq_t)5;   /* 2 for n odd, 5 for n even */

    /* Magnitudes of the negative terms */
    seq_t term2_mag = coeff2 * fn1;                  /* |term2| */

    seq_t term1_mag = 0;
    if (n & 1) {                                    /* n is odd */
        /* |term1| = 4 * (F(n) - 1) */
        term1_mag = 4 * (fn - 1);
    }

    /* 5 * F(n+1)^2 */
    seq_t term3 = fn1 * fn1;
    term3 = term3 * 5; /* 5 * (F(n+1))^2 */

    /* Combine the terms: term3 - term2_mag - term1_mag */
    seq_t result = term3;
    result -= term2_mag;
    result -= term1_mag;

    return result;
}
