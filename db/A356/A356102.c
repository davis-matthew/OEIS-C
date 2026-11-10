/*
 * A356102  Intersection of A001950 and A022839.
 * https://oeis.org/A356102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th term common to A001950(k)=floor(k*phi^2) and
 *            A022839(j)=floor(j*sqrt(5)); found by merging the two increasing
 *            sequences
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t A356102_upper(idx_t k) {
    seq_t m = (seq_t)k;
    seq_t s = isqrt_seq(5 * m * m);
    return m + (m + s) / 2;
}

static seq_t A356102_beatty5(idx_t j) {
    seq_t m = (seq_t)j;
    return isqrt_seq(5 * m * m);
}

seq_t a(idx_t n) {
    idx_t i = 1, j = 1, cnt = 0;
    seq_t u = A356102_upper(i);
    seq_t v = A356102_beatty5(j);
    for (;;) {
        if (u == v) {
            cnt++;
            if (cnt == n) return u;
            i++; j++;
            u = A356102_upper(i);
            v = A356102_beatty5(j);
        } else if (u < v) {
            i++;
            u = A356102_upper(i);
        } else {
            j++;
            v = A356102_beatty5(j);
        }
    }
}
