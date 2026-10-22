/*
 * A390501  a(n) = floor((4^n - 3^n - n + 2)/2).
 * https://oeis.org/A390501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((4^n - 3^n - n + 2)/2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0. */
    if (n == 0u) return 0;

    /* Convert n to seq_t for use with big_pow and arithmetic. */
    seq_t sn = (seq_t)n;

    /* Compute 4^n and 3^n using the provided helper. */
    seq_t pow4 = big_pow((seq_t)4, sn);
    seq_t pow3 = big_pow((seq_t)3, sn);

    /* Compute the numerator: 4^n - 3^n - n + 2. */
    seq_t diff = pow4 - pow3;      /* 4^n - 3^n  (non‑negative) */
    diff -= sn;                    /* subtract n     */
    diff += (seq_t)2;              /* add 2          */

    /* Perform integer division by 2 to get the floor. */
    seq_t result = diff / (seq_t)2;
    return result;
}
