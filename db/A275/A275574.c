/*
 * A275574  ((-1)^n - 1 + 2*(n^floor((n + 1)/2)))/4.
 * https://oeis.org/A275574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^⌊(n+1)/2⌋ - (n mod 2)) / 2 (where n = 1,2,3,...)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so we ignore n = 0 (could be returned as 0). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* k = floor((n+1)/2) */
    idx_t k = (n + 1) >> 1;

    /* Compute n^k as a big integer */
    seq_t pow = big_pow((seq_t)n, (seq_t)k);

    /* Subtract 1 if n is odd, otherwise subtract 0 */
    seq_t sub = (seq_t)(n & (idx_t)1);  /* 1 for odd, 0 for even */

    /* Divide by 2 to finish the formula */
    return (pow - sub) / (seq_t)2;
}
