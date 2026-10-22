/*
 * A358439  Number of even digits necessary to write all positive n-digit
 *          integers.
 * https://oeis.org/A358439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(9*n-1)*10^(n-2), a(1)=4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;

    seq_t term = (seq_t)9 * n - 1;   /* 9*n-1 */
    term = term * (seq_t)5;          /* 5*(9*n-1) */
    seq_t pow = pow10(n - 2);        /* 10^(n-2) */
    return term * pow;
}
