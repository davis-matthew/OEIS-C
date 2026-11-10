/*
 * A247431  The largest integer m such that A001950(m) < A003231(n).
 * https://oeis.org/A247431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(T/phi^2) with T = A003231(n) =
 *            floor(n*(sqrt(5)+5)/2); i.e. a(n) = (3T - isqrt(5T^2) - 1)/2
 *            (floor)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n)
{
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 s0 = isqrt_u128(5 * N * N);
    unsigned __int128 T = 2 * N + (N + s0) / 2;      /* A003231(n) */
    unsigned __int128 s = isqrt_u128(5 * T * T);
    unsigned __int128 r = (3 * T - s - 1) / 2;
    return (seq_t)(uint64_t)r;
}
