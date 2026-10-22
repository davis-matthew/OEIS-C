/*
 * A392050  a(n) is the denominator of the harmonic mean of the digits of n.
 * https://oeis.org/A392050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A392050: a(n) = denominator of the harmonic mean of the digits of n. */
/* For a number n, let d_1,…,d_k be its decimal digits. If any digit is 0, a(n)=1 (including n=0). */
/* Otherwise, harmonic mean HM = k / Σ(1/d_i). The fraction HM is reduced; a(n) is its denominator. */
/* Computation is purely rational arithmetic using gcd from utils. */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the special case n==0 or any zero digit. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum_num = 0;   /* Numerator of Σ(1/d_i). */
    seq_t sum_den = 1;   /* Denominator of Σ(1/d_i). */
    seq_t k = 0;         /* Count of digits. */

    uint64_t temp = (uint64_t)n;
    int has_zero = 0;

    /* Process digits from least to most significant. */
    while (temp > 0) {
        uint64_t d = temp % 10;
        temp /= 10;

        k++;

        if (d == 0) {
            has_zero = 1;
            /* No need to finish; we only need to return 1. */
            continue;
        }

        /* Add 1/d to sum_num/sum_den. */
        seq_t new_num = sum_num * (seq_t)d + sum_den * (seq_t)1;
        seq_t new_den = sum_den * (seq_t)d;

        seq_t g = gcd(new_num, new_den);
        if (g > 1) {
            new_num /= g;
            new_den /= g;
        }
        sum_num = new_num;
        sum_den = new_den;
    }

    if (has_zero)
        return (seq_t)1;

    /* Compute HM = k / (sum_num / sum_den) = k * sum_den / sum_num. */
    seq_t numHM = k * sum_den;
    seq_t denHM = sum_num;

    seq_t g2 = gcd(numHM, denHM);
    if (g2 > 1)
        denHM /= g2;

    return denHM;
}
