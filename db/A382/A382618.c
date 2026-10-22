/*
 * A382618  a(n) = 3^(n-2)*(binomial(n,2) + 3*n + 9).
 * https://oeis.org/A382618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-2) * ( binomial(n,2) + 3*n + 9 )
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"                      /* seq_pow(seq_t, idx_t) */

seq_t a(idx_t n)
{
    /* Handle the first two indices explicitly, because n-2 would underflow. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* binomial(n,2) = n*(n-1)/2  */
    seq_t bin = (seq_t)n * ((seq_t)n - 1) / 2;

    /* Linear part 3*n + 9 */
    seq_t linear = 3 * (seq_t)n + 9;

    /* 3^(n-2), safe because n >= 2 */
    seq_t pow3 = seq_pow((seq_t)3, n - 2);

    /* Final result */
    return pow3 * (bin + linear);
}
