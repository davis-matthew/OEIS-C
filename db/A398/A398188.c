/*
 * A398188  a(n) = Sum_{k=0..n} A398187(n, k).
 * https://oeis.org/A398188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..n} S(m), where S(m) = Sum_{j=0..m} (m!/j!)*n^j,
 *            computed by S(0) = 1, S(m) = m*S(m-1) + n^m
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t S = 1;
    seq_t pw = 1;
    seq_t total = 1;
    for (idx_t m = 1; m <= n; ++m) {
        pw *= N;
        S = (seq_t)m * S + pw;
        total += S;
    }
    return total;
}
