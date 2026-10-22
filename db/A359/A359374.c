/*
 * A359374  Parity of A252463(n).
 * https://oeis.org/A359374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pattern[(n-1) mod 12], with pattern =
 *            {1,1,0,0,1,1,1,0,0,1,1,0}
 * Range:     u8 n <= 9473, u16 n <= 9473, u32 n <= 9473, u64 n <= 9473,
 *            u128 n <= 9473, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pattern[12] = {1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0};
    return pattern[(size_t)((n - 1) % 12)];
}
