/*
 * A334998  Permutation of nonnegative integers that converts A162912 into
 *          A162911.
 * https://oeis.org/A334998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2k+1) = 2k for k >= 1; a(2k) = 2*b(k) + 1, where b
 *            is the inverse permutation of a: b(1) = 1, b(2k) = 2k+1 for k >=
 *            1, b(2k+1) = 2*a(k) for k >= 1. (Derived from A162912(a(n)) =
 *            A162911(n); the recursion halves its argument each step.)
 * Range:     u8 n <= 255, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 79 terms through n = 3770 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A334998_inv(uint64_t k);

static uint64_t A334998_fwd(uint64_t n)
{
    if (n <= 1) return n;
    if (n & 1) return n - 1;
    return 2 * A334998_inv(n >> 1) + 1;
}

static uint64_t A334998_inv(uint64_t k)
{
    if (k <= 1) return k;
    if ((k & 1) == 0) return k + 1;
    return 2 * A334998_fwd(k >> 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return (seq_t)A334998_fwd((uint64_t)n);
}
