/*
 * A359259  a(n) is the least k such that A359194(k) = A032766(n).
 * https://oeis.org/A359259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k with A359194(k) = A032766(n) = floor(3n/2);
 *            A359194(k) = 2^L - 1 - 3k, L = bitlength(3k). Computed directly:
 *            smallest L > bitlength(v) with 3 | (2^L - 1 - v), then k = (2^L
 *            - 1 - v)/3 (special case v = 1 gives k = 0)
 * Range:     u8 n <= 42, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t v = ((seq_t)n * 3) / 2;

    if (v == 1)
        return 0;

    unsigned len = 0;
    seq_t t = v;
    while (t != 0) {
        t >>= 1;
        ++len;
    }

    unsigned L = len + 1;
    for (;;) {
        seq_t p = ((seq_t)1 << L) - 1;
        seq_t d = p - v;
        if (d % 3 == 0)
            return d / 3;
        ++L;
    }
}
