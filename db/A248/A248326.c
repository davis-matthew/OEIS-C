/*
 * A248326  Square array read by downward antidiagonals: super Patalan numbers
 *          of order 5.
 * https://oeis.org/A248326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are coordinates of index n read by *
 *            downward antidiagonals. To find the coordinates: * s =
 *            ⌊(√(8*n+1) – 1)/2⌋ * i = n – s*(s+1)/2 (row index) * j = s – i
 *            (column index) * Then T(0,0)=1 and * T(n,k) =
 *            T(n-1,k)*(25*n-5)/(n+k) * T(n,k) = T(n,k-1)*(25*k-20)/(n+k) *
 *            The func...
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 35, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* multiply a by num and divide by denom exactly, avoiding overflow
 * when seq_t is an unsigned integer type that can hold the result. */
static seq_t mul_div(seq_t a, seq_t num, seq_t denom)
{
    seq_t g = gcd(a, denom);
    a /= g;
    denom /= g;
    g = gcd(num, denom);
    num /= g;
    denom /= g;      /* after reductions denom should be 1 */
    return a * num;
}

/* T(n,k) for n,k >= 0 */
static seq_t T_of(idx_t n, idx_t k)
{
    seq_t val = 1;

    /* compute column k of row 0: T(0,k) */
    for (idx_t t = 1; t <= k; ++t) {
        seq_t num   = 25 * t - 20;  /* numerator (25*t-20) */
        seq_t denom = t;            /* denominator t   */
        val = mul_div(val, num, denom);
    }

    /* now compute T(n,k) by adding n downward steps */
    for (idx_t t = 1; t <= n; ++t) {
        seq_t num   = 25 * t - 5;     /* numerator (25*t-5) */
        seq_t denom = t + k;          /* denominator (t+k)  */
        val = mul_div(val, num, denom);
    }

    return val;
}

/* public entry point */
seq_t a(idx_t n)
{
    /* find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    idx_t i = n - s * (s + 1) / 2;  /* row index (first element of diagonal) */
    idx_t j = s - i;                /* column index */

    return T_of(i, j);
}
