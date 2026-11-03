/*
 * A305063  a(n) = 110*2^n + 118.
 * https://oeis.org/A305063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110*2^n + 118
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);          /* v = 2^n */
    v = v * (seq_t)110;         /* v = 110*2^n */
    v = v + (seq_t)118;         /* v = 110*2^n + 118 */
    return v;
}
