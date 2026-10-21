/*
 * A266538  Twice the partial sums of A006257 (Josephus problem).
 * https://oeis.org/A266538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2 * sum_{i=1..n} (2*(i - 2^⌊log2 i⌋) + 1)
 * Range:     u8 n <= 21, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   highest_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   i.e. twice the partial sums of the Josephus numbers J(3,i). */

#include <stdint.h>
#include "../utils/highest_pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t p = highest_pow2(i);               /* 2^floor(log2 i) */
        seq_t j = 2 * ((seq_t)i - (seq_t)p) + 1;  /* J(3,i) */
        sum += j;
    }
    return 2 * sum;  /* twice the partial sum */
}
