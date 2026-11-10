/*
 * A385439  Row sums of triangle A385865.
 * https://oeis.org/A385439
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..n-1} ((j^2-1) mod n) + 1, i.e. the row sum of
 *            A385865 (column of j^2 in an n X n rowwise grid)
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t s = 0;
    for (idx_t j = 1; j < n; j++) {
        seq_t J = (seq_t)j;
        s += ((J * J - (seq_t)1) % N) + (seq_t)1;
    }
    return s;
}
