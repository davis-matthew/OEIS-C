/*
 * A268821  Permutation of nonnegative integers: a(0) = 0, a(n) = A268717(1 +
 *          A268717(n-1)).
 * https://oeis.org/A268821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = A268717(1 + A268717(n-1)); A268717(0)=0,
 *            A268717(m) = A003188(1 + A006068(m-1))
 * Range:     u8 n <= 129, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 100 terms through n = 8186 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline uint64_t A268821_gray(uint64_t x)
{
    return x ^ (x >> 1);
}

static inline uint64_t A268821_gray_inv(uint64_t g)
{
    uint64_t mask;
    for (mask = g >> 1; mask; mask >>= 1) {
        g ^= mask;
    }
    return g;
}

static uint64_t A268821_A268717(uint64_t m)
{
    if (m == 0) return 0;
    return A268821_gray(A268821_gray_inv(m - 1) + 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t inner = A268821_A268717((uint64_t)(n - 1));
    uint64_t res = A268821_A268717(inner + 1);
    return (seq_t)res;
}
