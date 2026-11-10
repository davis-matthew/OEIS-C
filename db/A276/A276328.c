/*
 * A276328  Digit sum when n is expressed in greedy A001563-base (A276326).
 * https://oeis.org/A276328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n >= 1, a(n) = 1 + a(n - m) where m = k*k! is the
 *            largest term of A001563 <= n (greedy)
 * Range:     u8 n <= 4320, u16 n <= 4320, u32 n <= 4320, u64 n <= 4320,
 *            u128 n <= 4320, big any n
 * Validated: standard, pass, 113 terms through n = 4238 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t t[20];
    uint64_t f = 1;
    for (int k = 1; k <= 19; k++) {
        f *= (uint64_t)k;
        t[k] = f * (uint64_t)k;
    }
    uint64_t r = (uint64_t)n;
    uint64_t count = 0;
    for (int k = 19; k >= 1; k--) {
        while (r >= t[k]) {
            r -= t[k];
            count++;
        }
    }
    return (seq_t)count;
}
