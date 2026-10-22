/*
 * A336899  a(n) = denominator(n / (4^n - 2^n)) for n > 0 and a(0) = 1.
 * https://oeis.org/A336899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n / (4^n - 2^n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* 2^n and 2^n-1 */
    seq_t two_n = pow2(n);
    seq_t two_n_minus1 = two_n - (seq_t)1;

    /* denominator before reduction */
    seq_t denom = two_n * two_n_minus1;

    /* compute v2(n) = exponent of 2 in n */
    idx_t tmp = n;
    int v2 = 0;
    while ((tmp & 1) == 0) {
        v2++;
        tmp >>= 1;
    }

    /* odd part of n */
    idx_t n_odd = n >> v2;

    /* gcd contribution */
    seq_t pow2_v2 = pow2((idx_t)v2);
    seq_t g_odd = seq_gcd((seq_t)n_odd, two_n_minus1);
    seq_t g = pow2_v2 * g_odd;

    /* final denominator after reducing with n */
    seq_t result = denom / g;
    return result;
}
