/*
 * A302793  Permutation of nonnegative integers: a(0) = 0; for n >= 1, a(n) =
 *          A193231(1+A193231(n-1)), where A193231(n) is blue code of n.
 * https://oeis.org/A302793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n >= 1, a(n) = A193231(1+A193231(n-1))
 * Range:     u8 n <= 128, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 98 terms through n = 9574 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A193231_a(seq_t n)
{
    seq_t res  = 0;
    seq_t mask = 1;
    seq_t t = n;

    while (t != 0) {
        if ((t & 1) != 0)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t b = A193231_a((seq_t)(n - 1));
    return A193231_a(b + 1);
}
