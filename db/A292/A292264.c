/*
 * A292264  a(n) = n - A292944(n).
 * https://oeis.org/A292264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - A292944(n), where A292944(n) keeps the 2nd most
 *            significant 1-bit of n and any lower 1-bit whose left neighbour
 *            is 0, and clears the msb and all other 1-bits
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 110 terms through n = 9005 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t A292264_msb_idx(idx_t n)
{
    idx_t pos = 0;
    while (n >>= 1) ++pos;
    return pos;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;

    idx_t m = A292264_msb_idx(n);
    idx_t temp = n & ~((idx_t)1 << m);
    idx_t kept = 0;
    if (temp != 0) {
        idx_t s = A292264_msb_idx(temp);
        for (idx_t i = 0; i < m; ++i) {
            if (((n >> i) & 1u) == 0) continue;
            if (i == s || ((n >> (i + 1)) & 1u) == 0)
                kept |= ((idx_t)1 << i);
        }
    }
    return (seq_t)(n - kept);
}
