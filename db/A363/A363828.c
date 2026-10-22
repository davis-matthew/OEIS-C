/*
 * A363828  Highest power of 2 dividing n which is < sqrt(n), for n >= 2; a(1)
 *          = 1.
 * https://oeis.org/A363828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of 2 dividing n that is < sqrt(n), a(1)=1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;

    /* Compute the exponent of 2 in n */
    idx_t temp = n;
    idx_t v2 = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++v2;
    }

    /* Floor(sqrt(n)), adjusted for perfect squares */
    idx_t sqrt_floor = _floor_isqrt_u64(n);
    if (sqrt_floor * sqrt_floor == n) {
        if (sqrt_floor > 0) sqrt_floor--;
    }

    seq_t limit = (seq_t)sqrt_floor;

    /* Compute the largest power of 2 dividing n as a 64‑bit value */
    uint64_t pow = 1ULL << v2;
    while (pow > (uint64_t)limit) {
        pow >>= 1;
    }

    return (seq_t)pow;
}
