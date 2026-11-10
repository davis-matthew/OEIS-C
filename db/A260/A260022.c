/*
 * A260022  A bisection of A006921.
 * https://oeis.org/A260022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006921(2*n) = Sum_{r=0..n} (binomial(2n-r,r) mod 2) *
 *            2^(n-r)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 987 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t m = 2 * n;
    for (idx_t r = 0; r <= n; r++) {
        result = result * 2;
        if (((m - r) & r) == r) {
            result = result + 1;
        }
    }
    return result;
}
