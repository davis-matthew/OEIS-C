/*
 * A254311  Set of all natural numbers m such that m < S(m), where the
 *          function S is as defined in A257480.
 * https://oeis.org/A254311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m >= 1 with m < S(m), S as in A257480: S(m) =
 *            (3 + (3/2)^v(1+F(4m-3))*(1+F(4m-3)))/6, F(x) = (3x+1)/2^v(3x+1);
 *            found by linear search
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A254311_S(seq_t m) {
    seq_t x = (seq_t)4 * m - 3;
    seq_t t = 3 * x + 1;
    while ((t & 1) == 0) {
        t >>= 1;
    }
    seq_t q = t + 1;
    seq_t p3 = 1;
    while ((q & 1) == 0) {
        q >>= 1;
        p3 *= 3;
    }
    return (3 + p3 * q) / 6;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    while (count < n) {
        ++m;
        if (m < A254311_S(m)) {
            ++count;
        }
    }
    return m;
}
