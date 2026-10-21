/*
 * A259574  Sum of numbers in the n-th antidiagonal of the reciprocity array
 *          of 0.
 * https://oeis.org/A259574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} Sum_{k=0..m-1} floor(n*k/m)
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t k = 0; k < m; ++k) {
            seq_t nk = (seq_t)n * (seq_t)k;
            seq_t term = nk / (seq_t)m;
            total += term;
        }
    }
    return total;
}
