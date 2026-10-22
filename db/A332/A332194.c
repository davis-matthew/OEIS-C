/*
 * A332194  a(n) = 10^(2*n+1) - 1 - 5*10^n.
 * https://oeis.org/A332194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n+1) - 1 - 5*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Computes the n-th term of OEIS sequence A332194 */
seq_t a(idx_t n)
{
    /* 10^n */
    seq_t p10_n   = pow10(n);
    /* 10^(2*n+1) = 10^(2n+1) */
    seq_t p10_2n1 = pow10(n * 2 + 1);

    /* 5 * 10^n */
    seq_t five_p10_n = 5 * p10_n;

    /* a(n) = 10^(2n+1) - 1 - 5*10^n */
    seq_t result = p10_2n1 - 1;
    result -= five_p10_n;

    return result;
}
