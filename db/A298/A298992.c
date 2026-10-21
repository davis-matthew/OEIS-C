/*
 * A298992  a(n) = (2*n-3-(-1)^n)*(22*n^2-21*n+5*n*(-1)^n)/96.
 * https://oeis.org/A298992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-3-(-1)^n)*(22*n^2-21*n+5*n*(-1)^n)/96
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small cases directly */
    if (n <= 2)
        return 0;

    /* Determine (-1)^n */
    int sign = (n % 2 == 0) ? 1 : -1;

    /* Work in unsigned 64‑bit arithmetic to avoid overflow here. */
    unsigned long long N = (unsigned long long)n;

    /* t1 = 2*n - 3 - (-1)^n */
    unsigned long long t1 = 2ULL * N - 3ULL - (unsigned long long)sign;

    /* t2 = 22*n^2 - 21*n + 5*n*(-1)^n */
    unsigned long long t2;
    if (sign == 1) /* even n */
        t2 = 22ULL * N * N - 21ULL * N + 5ULL * N;
    else          /* odd n */
        t2 = 22ULL * N * N - 21ULL * N - 5ULL * N;

    /* Compute the product and divide by 96. */
    unsigned long long prod = t1 * t2;
    unsigned long long res  = prod / 96ULL;

    return (seq_t)res;
}
