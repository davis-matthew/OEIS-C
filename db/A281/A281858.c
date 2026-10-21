/*
 * A281858  Curious cubic identities based on the Armstrong number 370.
 * https://oeis.org/A281858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n * (1 + 10^n + 10^(2n)) / 3
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 10^(2n).  The exponent 2*n is safe because n <= 10000. */
    seq_t p10   = pow10(n);
    seq_t p10sq = pow10((idx_t)(2 * n));

    /* Sum the terms 1 + 10^n + 10^(2n). */
    seq_t sum = ((seq_t)1) + p10 + p10sq;

    /* Multiply by 10^n and divide by 3 (division is exact). */
    seq_t prod   = p10 * sum;
    seq_t result = prod / 3;

    return result;
}
