/*
 * A268817  Permutation of nonnegative integers: a(n) = A268717(A268717(n)).
 * https://oeis.org/A268817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A268717(A268717(n)), where A268717(0) = 0 and A268717(n)
 *            = A003188(1 + A006068(n-1))
 * Range:     u8 n <= 128, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 98 terms through n = 8149 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t A268717_gray(uint64_t x)
{
    return x ^ (x >> 1);
}

static inline uint64_t A268717_gray_to_binary(uint64_t g)
{
    uint64_t mask;
    for (mask = g >> 1; mask; mask >>= 1) {
        g ^= mask;
    }
    return g;
}

static uint64_t A268717_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t inv = A268717_gray_to_binary(n - 1);
    return A268717_gray(inv + 1);
}

seq_t a(idx_t n)
{
    return (seq_t)A268717_u64(A268717_u64((uint64_t)n));
}
