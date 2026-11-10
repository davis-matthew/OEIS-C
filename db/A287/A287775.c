/*
 * A287775  Positions of 0 in A287772; complement of A050140 (conjectured and
 *          proved).
 * https://oeis.org/A287775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer not of the form 2*floor(m*phi)-m
 *            (A050140), phi=(1+sqrt(5))/2; computed by a linear scan
 * Range:     u8 n <= 8, u16 n <= 227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 7527 (2026-11-08)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t A287775_b(seq_t m)
{
    seq_t s = isqrt_seq((seq_t)5 * m * m);
    seq_t f = (m + s) / 2;
    return 2 * f - m;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t x = 0;
    seq_t count = 0;
    seq_t m = 1;
    seq_t b = A287775_b(m);
    while (count < target) {
        x++;
        if (x == b) {
            m++;
            b = A287775_b(m);
        } else {
            count++;
        }
    }
    return x;
}
