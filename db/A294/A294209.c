/*
 * A294209  Sum of the smaller parts in the partitions of n into two
 *          squarefree parts or two nonsquarefree parts.
 * https://oeis.org/A294209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i * [A008966(i) = A008966(n-i)]
 * Range:     u8 n <= 51, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n >> 1;               /* floor(n/2) */
    for (idx_t i = 1; i <= half; ++i) {
        seq_t si = (seq_t)i;
        seq_t sj = (seq_t)(n - i);
        if (is_squarefree(si) == is_squarefree(sj)) {
            sum += si;
        }
    }
    return sum;
}
