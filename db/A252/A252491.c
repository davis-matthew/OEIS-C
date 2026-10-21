/*
 * A252491  a(n) = (10^(n^2) - 1)/(10^n - 1).
 * https://oeis.org/A252491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(n^2) - 1)/(10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Computes a(n) for the OEIS sequence A252491. */
seq_t a(idx_t n)
{
    /* Handle the trivial case for completeness. */
    if (n == 0)
        return 0;

    /* Compute 10^n and 10^(n^2) using the provided pow10 helper. */
    seq_t ten_pow_n   = pow10(n);
    seq_t denominator = ten_pow_n - 1;

    /* n^2 fits in idx_t for n up to about 10^9, which covers all test cases. */
    idx_t exp = n * n;
    seq_t ten_pow_exp = pow10(exp);
    seq_t numerator  = ten_pow_exp - 1;

    return numerator / denominator;
}
