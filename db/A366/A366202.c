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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A366202: a(n) is the n-th positive integer x such that (x^2 & x) is NOT a perfect square. */
/* This implementation scans all positive integers x, computing (x^2 & x) with arbitrary-precision arithmetic
   (or 64‑bit unsigned arithmetic when seq_t is uint64_t). The floor_sqrt utility is used to test for perfect squares. */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

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
