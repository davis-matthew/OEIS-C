/*
 * A295175  Chromatic invariant of the n-Hanoi graph.
 * https://oeis.org/A295175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^e(n) where e(1)=0 and e(n) = 3*e(n-1) + 6*(n-2) for
 *            n>=2
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t exp = 0;
    for (idx_t i = 2; i <= n; ++i) {
        exp = exp * 3 + (seq_t)(6 * (i - 2));
    }
    return ipow((seq_t)2, exp);
}
