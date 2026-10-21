/*
 * A283437  Periodic {1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1}.
 * https://oeis.org/A283437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic{1,1,1,1,0,1,0,0,1,0,1,1}
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[12] = {1,1,1,1,0,1,0,0,1,0,1,1};
    return pattern[(size_t)(n % 12)];
}
