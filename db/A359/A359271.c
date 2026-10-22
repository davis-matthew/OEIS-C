/*
 * A359271  Number of odd digits necessary to write all nonnegative n-digit
 *          integers.
 * https://oeis.org/A359271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 * (9*n + 1) * 10^(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1)        /* offset is 1; n<1 is undefined, return 0 */
        return 0;
    if (n == 1)       /* 10^(–1) = 1/10, but the product is 5 */
        return 5;

    /* n >= 2  →  n‑2 >= 0 */
    seq_t result = pow10(n - 2);               /* 10^(n-2) */
    result *= ((seq_t)9 * (seq_t)n + 1);       /* (9*n + 1) */
    result *= 5;                               /* *5 */
    return result;
}
