/*
 * A360715  Number of self-avoiding paths with nodes chosen among n given
 *          points on a circle; one-node paths are allowed.
 * https://oeis.org/A360715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/4)*(3^(n-1) + 3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* outside the defined offset */

    /* 3^(n-1) + 3 */
    seq_t sum = pow3(n - 1) + (seq_t)3;

    seq_t n4   = (seq_t)(n / 4);          /* floor(n/4)   */
    seq_t rem  = (seq_t)(n % 4);          /* remainder   */

    /* (n/4) * sum  */
    seq_t result = seq_mul(sum, n4);

    /* add the remaining part: (n % 4) * sum / 4   */
    if (rem) {
        seq_t tmp = seq_mul(sum, rem);   /* small multiplication, no overflow */
        result = result + (tmp >> 2);    /* integer division by 4 */
    }

    return result;
}
