/*
 * A246987  Expansion of
 *          (1-11*x+34*x^2-21*x^3+2*x^4)/((1-x)*(1-2*x)*(1-4*x)*(1-5*x)*(1-6*x)).
 * https://oeis.org/A246987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 - 3*2^(2+n) + 7*2^(1+2*n) + 3*5^n + 6^(1+n)) / 12
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   power_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power_uint128.h"

/* ---- Power helpers ----------------------------------------------------- */

/* Power for larger seq_t (≥128 bits). Uses seq_t arithmetic directly. */
static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp)
            base *= base;
    }
    return result;
}

/* ---- Sequence function ------------------------------------------------- */

seq_t a(idx_t n)
{
    /* If seq_t is 64–bit or smaller, use __int128 intermediates. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        __int128 pow2_2n     = power_uint128(2, 2 + n);   /* 2^(2+n)   */
        __int128 pow2_1_2n   = power_uint128(2, 1 + 2 * n);/* 2^(1+2n)  */
        __int128 pow5_n      = power_uint128(5, n);       /* 5^n       */
        __int128 pow6_1n     = power_uint128(6, 1 + n);   /* 6^(1+n)   */

        __int128 numerator = 1
                             - 3 * pow2_2n
                             + 7 * pow2_1_2n
                             + 3 * pow5_n
                             +     pow6_1n;

        /* The division is exact for all n. */
        return (seq_t)(numerator / 12);
    }
    else
    {
        /* seq_t is large (≥128‑bit).  Compute directly with seq_t arithmetic. */
        seq_t pow2_2n     = power_seq_t((seq_t)2, 2 + n);
        seq_t pow2_1_2n   = power_seq_t((seq_t)2, 1 + 2 * n);
        seq_t pow5_n      = power_seq_t((seq_t)5, n);
        seq_t pow6_1n     = power_seq_t((seq_t)6, 1 + n);

        seq_t numerator = 1
                          - 3 * pow2_2n
                          + 7 * pow2_1_2n
                          + 3 * pow5_n
                          +     pow6_1n;

        return numerator / 12;
    }
}
