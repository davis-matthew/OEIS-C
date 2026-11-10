/*
 * A266641  Permutation of nonnegative integers: a(n) = A264965(2*n) / 2.
 * https://oeis.org/A266641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A264965(2*n) / 2, where A264965(m) = adjusted base-3
 *            reverse of the adjusted base-2 reverse of m (trailing zeros kept
 *            fixed)
 * Range:     u8 n <= 123, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 98 terms through n = 9444 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A266641_rev_base(seq_t n, unsigned b)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)b) == (seq_t)0) { x /= (seq_t)b; ++t; }
    seq_t rev = (seq_t)0;
    while (x > (seq_t)0) {
        rev = rev * (seq_t)b + (x % (seq_t)b);
        x /= (seq_t)b;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)b;
    return rev;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)2;
    seq_t r = A266641_rev_base(A266641_rev_base(m, 2), 3);
    return r / (seq_t)2;
}
