/*
 * A359268  a(n) is the least k such that A359194(k) = A359194(n).
 * https://oeis.org/A359268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that A359194(k) = A359194(n), where
 *            A359194(n) = (2^L - 1) - 3n with L = max(1, bitlength(3n));
 *            found by searching L upward
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 86 terms through n = 8095 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static unsigned A359268_bitlen(seq_t x)
{
    unsigned len = 0;
    while (x > 0) {
        x >>= 1;
        ++len;
    }
    if (len == 0) len = 1;
    return len;
}

static seq_t A359268_mask(unsigned L)
{
    /* 2^L - 1 without overflow at full width */
    return ((((seq_t)1) << (L - 1)) - 1) * 2 + 1;
}

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n) * 3;
    unsigned len = A359268_bitlen(m);
    seq_t v = A359268_mask(len) - m;

    for (unsigned L = 1; L <= len; ++L) {
        seq_t mask = A359268_mask(L);
        if (mask < v) continue;
        seq_t t = mask - v;
        if (t % 3 != 0) continue;
        if (A359268_bitlen(t) != L) continue;
        return t / 3;
    }
    return (seq_t)n;
}
