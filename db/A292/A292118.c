/*
 * A292118  G.f.: 1 + 2*Sum_{k >= 1} (-1)^k*q^A003159(k).
 * https://oeis.org/A292118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1 + 2*Sum_{k>=1} (-1)^k*q^A003159(k): a(0)=1; for m>=1, if
 *            m has an even number of trailing zeros in binary then a(m) =
 *            2*(-1)^k, where k = #{j <= m : j in A003159} = Sum_{i>=0}
 *            floor((floor(m/4^i)+1)/2); otherwise a(m) = 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 8995 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A292118_count(uint64_t m) {
    uint64_t c = 0;
    while (m > 0) {
        c += (m + 1) / 2;
        m >>= 2;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t m = (uint64_t)n;
    int z = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        z++;
    }
    if (z & 1) return (seq_t)0;
    uint64_t k = A292118_count((uint64_t)n);
    return (k & 1) ? (seq_t)(-2) : (seq_t)2;
}
