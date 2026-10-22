/*
 * A320153  Number of integer solutions to a^2 + 2*b^2 + 2*c^2 + 7*d^2 = n.
 * https://oeis.org/A320153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count integer solutions of a^2 + 2*b^2 + 2*c^2 + 7*d^2 = n,
   accounting for sign multiplicities. */
#include <stdint.h>

/* Integer square‑root, floor, for 64‑bit values.  */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    idx_t max_d = isqrt_u64(n / 7);
    for (idx_t d = 0; d <= max_d; ++d) {
        idx_t val_d  = 7 * d * d;
        idx_t rem_d  = n - val_d;

        idx_t max_c = isqrt_u64(rem_d / 2);
        for (idx_t c = 0; c <= max_c; ++c) {
            idx_t val_dc = val_d + 2 * c * c;
            idx_t rem_dc = n - val_dc;

            idx_t max_b = isqrt_u64(rem_dc / 2);
            for (idx_t b = 0; b <= max_b; ++b) {
                idx_t val_dcb = val_dc + 2 * b * b;
                idx_t rem     = n - val_dcb;

                idx_t a_sq = isqrt_u64(rem);
                if (a_sq * a_sq == rem) {
                    seq_t mult = 1;
                    if (d != 0) mult <<= 1;
                    if (c != 0) mult <<= 1;
                    if (b != 0) mult <<= 1;
                    if (a_sq != 0) mult <<= 1;
                    result += mult;
                }
            }
        }
    }
    return result;
}
