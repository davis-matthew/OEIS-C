/*
 * A396250  a(n) is the number of ternary sequences of length n with
 *          characteristic value 0, as is defined in comments.
 * https://oeis.org/A396250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * (3^(2^popcnt(n-1)) + 1)/(2*3^(2^popcnt(n-1))) -
 *            2^(n-1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* n is 1‑based, offset = 1 */
    if (n == 0) return 0;                      /* safety, not used for offset 1 */

    /* compute popcount of n-1 */
    idx_t x = n - 1;
    int popcnt = 0;
    while (x) {
        popcnt++;
        x &= x - 1;                             /* clear lowest set bit */
    }

    /* M = 2^popcnt */
    idx_t M = ((idx_t)1) << popcnt;            /* popcnt <= 63 for any n <= 2^63-1 */

    /* C = 3^(n-M), B = 3^M */
    seq_t C = pow_seq((seq_t)3, (int)(n - M));
    seq_t B = pow_seq((seq_t)3, (int)M);

    /* (C * (B + 1)) / 2  */
    seq_t temp = (B + (seq_t)1);
    temp = C * temp;
    seq_t half = temp / 2;

    /* subtract 2^(n-1) */
    seq_t two_pow = pow_seq((seq_t)2, (int)(n - 1));

    return half - two_pow;
}
