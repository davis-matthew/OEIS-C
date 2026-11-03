/*
 * A256136  a(n) = 6^A000120(n).
 * https://oeis.org/A256136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^popcount(n)
 * Range:     u8 n <= 14, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    unsigned int pc = 0;
    idx_t m = n;
    while (m)
    {
        m &= m - 1;
        pc++;
    }
    return ipow_idx((seq_t)6, (idx_t)pc);
}
