/*
 * A283188  A periodic sequence of 8-bit binary numbers for single-bit
 *          multi-frequency generation.
 * https://oeis.org/A283188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..8} (floor((n - k)/k) mod 2) * 2^(8 - k)
 *            Simplified: a(n) = Sum_{k=1..8} (1 - ((n/k) & 1)) * 2^(8 - k)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= 8; ++k) {
        idx_t q   = n / k;            /* floor(n/k) for non‑negative n */
        int bit   = ((q & 1) ? 0 : 1); /* 1 if q even, else 0 */
        res |= ((seq_t)bit) << (8 - k);
    }
    return res;
}
