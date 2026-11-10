/*
 * A356057  a(n) = A001951(A137804(n)).
 * https://oeis.org/A356057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001951(A137804(n)), where A137804(n) =
 *            floor(n*(4*sqrt(2)+9)/7) and A001951(m) = floor(m*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-10)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

static seq_t a356057_isqrt(seq_t x)
{
    if (x == 0) return 0;
    unsigned bits = bit_length(x);
    seq_t low = 0;
    seq_t high = ((seq_t)1) << ((bits + 1) / 2);
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        if (mid * mid <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* floor((9n + 4n*sqrt2)/7) = floor((9n + floor(sqrt(32 n^2)))/7) */
    seq_t q = (9 * N + a356057_isqrt(32 * N * N)) / 7;
    /* floor(q*sqrt2) = isqrt(2 q^2) */
    return a356057_isqrt(2 * q * q);
}
