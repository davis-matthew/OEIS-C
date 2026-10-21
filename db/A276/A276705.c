/*
 * A276705  Records in A249860.
 * https://oeis.org/A276705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (347+27*(-1)^n)/2 - 3*(27+(-1)^n)*n + 9*n^2 for n > 6
 *            Initial values (n = 1..6): a(1)=4, a(2)=5, a(3)=7, a(4)=8,
 *            a(5)=9, a(6)=20
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* direct look‑up for the first six terms */
    if (n <= 6) {
        static const seq_t init[] = {0, 4, 5, 7, 8, 9, 20};
        return init[n];
    }

    /* compute the conjectured quadratic expression for n >= 7 */
    int sign = (n & 1) ? -1 : 1;   /* (-1)^n */
    long long ln   = (long long)n; /* safe for the ranges tested */
    long long res  = (347 + 27 * sign) / 2
                  - 3LL * (27 + sign) * ln
                  + 9LL * ln * ln;

    return (seq_t)res;
}
