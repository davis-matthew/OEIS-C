/*
 * A379417  a(n) = n + [n*s/r] + [n*t/r], where r = (3/2)^(1/4); s =
 *          (3/2)^(1/2); t = (3/2)^(3/4) and [ ] = floor.
 * https://oeis.org/A379417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*α) + floor(n*β), where α = (3/2)^(1/4), β =
 *            (3/2)^(1/2)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

seq_t a(idx_t n) {
    seq_t nseq = (seq_t) n;

    /* floor(n*β) = floor( sqrt( (3*n*n) / 2 ) ) */
    seq_t beta_num = ((seq_t)3 * nseq * nseq) / 2;
    seq_t beta_floor = floor_sqrt(beta_num);

    /* floor(n*α) = floor( (3*n^4 / 2)^(1/4) ) */
    seq_t target = ((seq_t)3 * nseq * nseq * nseq * nseq) / 2;
    seq_t low  = 0;
    seq_t high = nseq * 2 + 1;   /* an upper bound > floor(n*α) */

    while (low < high) {
        seq_t mid  = (low + high + 1) / 2;
        seq_t mid2 = mid * mid;
        seq_t mid4 = mid2 * mid2;
        if (mid4 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    seq_t alpha_floor = low;

    return nseq + alpha_floor + beta_floor;
}
