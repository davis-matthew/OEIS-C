/*
 * A368592  a(n) = numerator of -(1/4)*n!*(2 + n!)*(-2 + 1/(1 + floor(n/2 -
 *          1/2))) - n!*Sum_{m=1..1 + 2*floor(n/2 - 1/2)} 1/m.
 * https://oeis.org/A368592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of -(1/4)*n!*(2 + n!)*(-2 + 1/(1 + floor(n/2 -
 *            1/2))) - n!*Sum_{m=1..1 + 2*floor(n/2 - 1/2)} 1/m
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the offset term directly */
    if (n == 1) {
        return (seq_t)((long long)-1);
    }

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Determine M = 1 + 2*floor(n/2 - 1/2) = { n if n odd, n-1 if n even } */
    idx_t M = (n & 1) ? n : n - 1;

    /* Compute k+1 = 1 + floor(n/2 - 1/2) = { (n+1)/2 if n odd, n/2 if n even } */
    idx_t k_plus1 = (n & 1) ? ((n + 1) / 2) : (n / 2);

    /* T1 = n! * (2 + n!) * M / (4 * (k+1))  -- integer division guaranteed */
    seq_t denom = (seq_t)4 * (seq_t)k_plus1;
    seq_t temp = fact + (seq_t)2;          /* 2 + n! */
    seq_t prod = fact * temp;              /* n! * (2 + n!) */
    prod *= (seq_t)M;                      /* * M */
    seq_t T1 = prod / denom;               /* exact division */

    /* Compute S = sum_{m=1..M} n! / m  -- each term is integer */
    seq_t sum = 0;
    for (idx_t m = 1; m <= M; ++m) {
        seq_t term = fact / (seq_t)m;
        sum += term;
    }

    return T1 - sum;
}
