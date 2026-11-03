/*
 * A366202  Numbers x such that (x^2 AND x) is not a square, where AND is the
 *          bitwise logical-and operation.
 * https://oeis.org/A366202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 950 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A366202: a(n) is the n-th positive integer x such that (x^2 & x) is NOT a perfect square. */
/* This implementation scans all positive integers x, computing (x^2 & x) with arbitrary-precision arithmetic
   (or 64‑bit unsigned arithmetic when seq_t is uint64_t). The floor_sqrt utility is used to test for perfect squares. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for offset 0, return 0 */
    idx_t count = 0;
    seq_t x = (seq_t)1;           /* candidate x */

    for (;;) {
        /* y = (x^2) & x */
        seq_t prod = x * x;
        seq_t y = prod & x;

        /* check if y is a perfect square */
        seq_t s = floor_sqrt(y);
        if (s * s != y) {         /* not a square => record */
            ++count;
            if (count == n)
                return x;
        }
        ++x;                       /* move to next integer */
    }
}
