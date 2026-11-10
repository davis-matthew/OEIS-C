/*
 * A376357  Positions of numbers in A007961 that end in 0.
 * https://oeis.org/A376357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the n-th number m with A007961(m) ending in 0
 *            (found by scanning m = 1, 2, 3, ... and testing the last digit
 *            of A007961(m))
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A376357_ends0(idx_t m) {
    idx_t k = 1;
    while ((k + 1) * (k + 1) <= m) k++;
    idx_t rem = m;
    while (k >= 2) {
        idx_t sq = k * k;
        idx_t nsq = (k + 1) * (k + 1);
        idx_t maxd = (nsq - 1) / sq;
        idx_t d = rem / sq;
        if (d > maxd) d = maxd;
        rem -= d * sq;
        k--;
    }
    return rem == 0;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        m++;
        if (A376357_ends0(m)) count++;
    }
    return (seq_t)m;
}
