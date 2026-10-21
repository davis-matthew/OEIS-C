/*
 * A256136  a(n) = 6^A000120(n).
 * https://oeis.org/A256136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^popcount(n)
 * Range:     u8 n <= 14, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    unsigned int pc = 0;
    idx_t m = n;
    while (m)
    {
        m &= m - 1;
        pc++;
    }
    return pow_int((seq_t)6, (idx_t)pc);
}
