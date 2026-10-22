/*
 * A345125  Numerator of 4/(1 + 1^2/(3 + 2^2/(5 + 3^2/(7 + ... +
 *          (n-1)^2/(2*n-1) )))).
 * https://oeis.org/A345125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 4 / (1 + 1²/(3 + 2²/(5 + 3²/(7 + … +
 *            (n‑1)²/(2n‑1) )))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 43,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute the n-th term of A345125.
   a(0) = 0, a(1) = 4. */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;
    if (n == 1u) return 4;

    /* Start from the deepest layer:
       value = (n-1)² / (2n-1). */
    seq_t num = (seq_t)(n - 1u);
    num = num * num;                      /* (n-1)² */
    seq_t den = (seq_t)(2u * n - 1u);      /* 2n-1 */

    /* Expand outward, decreasing k from n-2 down to 1. */
    for (idx_t k = n - 2u; k > 0u; --k)
    {
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t new_num = ksq * den;
        seq_t new_den = den * ((seq_t)(2u * k + 1u)) + num;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* Now the nested fraction C = num / den.
       Compute 4 / (1 + C) = 4*den / (den + num). */
    seq_t full_num = (seq_t)4 * den;
    seq_t full_den = den + num;
    seq_t g2 = gcd(full_num, full_den);
    return full_num / g2;  /* numerator after reduction */
}
