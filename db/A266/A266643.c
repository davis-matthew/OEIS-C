/*
 * A266643  Permutation of nonnegative integers: a(n) = A264965(3*n) / 3.
 * https://oeis.org/A266643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A264965(3*n) / 3, where A264965(m) = A263273(A057889(m))
 *            (adjusted base-2 reverse, then adjusted base-3 reverse)
 * Range:     u8 n <= 52, u16 n <= 10921, u32 n <= 10921, u64 n <= 10921,
 *            u128 n <= 10921, big any n
 * Validated: standard, pass, 98 terms through n = 9700 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A264965_rev2(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)2) == (seq_t)0) { x /= (seq_t)2; ++t; }
    seq_t rev = (seq_t)0;
    seq_t y = x;
    while (y > (seq_t)0) {
        rev = rev * (seq_t)2 + (y % (seq_t)2);
        y /= (seq_t)2;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)2;
    return rev;
}

static seq_t A264965_rev3(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)3) == (seq_t)0) { x /= (seq_t)3; ++t; }
    seq_t rev = (seq_t)0;
    seq_t y = x;
    while (y > (seq_t)0) {
        rev = rev * (seq_t)3 + (y % (seq_t)3);
        y /= (seq_t)3;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)3;
    return rev;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)3;
    seq_t b2 = A264965_rev2(m);
    seq_t r = A264965_rev3(b2);
    return r / (seq_t)3;
}
