/*
 * A384192  Complement of A384191.
 * https://oeis.org/A384192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 3 * ⌊ ⌊ k·φ ⌋ ⌋, where φ = (1+√5)/2 and k = ⌈ n/3 ⌉.
 *            The inner floor can be computed as floor( (k + floor(k·√5)) / 2
 *            ). Here floor_sqrt returns ⌊√x⌋ for integer x
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Compute k = ceil(n/3) = (n-1)/3 + 1 (all are unsigned) */
    idx_t k = (n - 1) / 3 + 1;
    seq_t kk = (seq_t)k;

    /* t = ⌊k · √5⌋  computed as ⌊√(5·k²)⌋ */
    seq_t t = floor_sqrt((seq_t)5 * kk * kk);

    /* fk = ⌊k · φ⌋ = ⌊(k + t) / 2⌋ */
    seq_t fk = (kk + t) / 2;

    /* a(n) = n + 3 * fk  */
    return (seq_t)n + (seq_t)3 * fk;
}
