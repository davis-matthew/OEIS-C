/*
 * A255191  Number of solutions to the equation n = x^2 + y*(y+1)/2 +
 *          z*(z+1)/2 (x,y,z=0,1,...) with x == Floor[sqrt(n)] (mod 2) and y
 *          <= z.
 * https://oeis.org/A255191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (x,y,z) | n = x^2 + y(y+1)/2 + z(z+1)/2, x == ⌊√n⌋
 *            (mod 2), y ≤ z, x,y,z≥0 }|
 * Range:     u8 n <= 10, u16 n <= 469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force enumeration.  For n≤10000 the
   routine runs in well under two seconds. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"        /* isqrt_seq(seq_t) → seq_t */

/* Integer square root for idx_t (uint64_t). */
static inline idx_t u64_isqrt(idx_t n)
{
    idx_t low = 0, high = n, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n_idx)
{
    seq_t count = 0;
    seq_t n = (seq_t)n_idx;

    /* floor(sqrt(n)) for parity reference */
    idx_t sqrt_n = u64_isqrt(n_idx);
    idx_t parity_ref = sqrt_n & 1UL;

    for (idx_t x = 0; x <= sqrt_n; ++x) {
        if ((x & 1UL) != parity_ref) continue;     /* parity mismatch */

        seq_t x_sq = (seq_t)x * (seq_t)x;
        seq_t rem_xy = n - x_sq;
        /* Enumerate y; compute z from the remaining triangular part */
        for (idx_t y = 0; ; ++y) {
            seq_t T_y = (seq_t)y * (seq_t)(y + 1) / 2;
            if (T_y > rem_xy) break;              /* no further valid y */

            seq_t rem = rem_xy - T_y;             /* must be triangular */
            seq_t one_plus_8rem = (seq_t)1 + (seq_t)8 * rem;
            seq_t t_seq = isqrt_seq(one_plus_8rem);

            /* Check that 1 + 8*rem is a perfect square */
            if (t_seq * t_seq != one_plus_8rem) continue;

            seq_t z_seq = (t_seq - (seq_t)1) / 2;
            if (z_seq < (seq_t)y) continue;        /* enforce y ≤ z */

            ++count;
        }
    }
    return count;
}
