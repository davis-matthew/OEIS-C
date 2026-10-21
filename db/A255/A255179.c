/*
 * A255179  Second differences of ninth powers (A001017).
 * https://oeis.org/A255179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=6*n*(3+28*n^2+42*n^4+12*n^6) for n>0
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 12, u64 n <= 306, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t nt = (seq_t)n;
    seq_t n2 = nt * nt;
    seq_t n4 = n2 * n2;
    seq_t n6 = n4 * n2;
    seq_t term = nt * (3 + 28 * n2 + 42 * n4 + 12 * n6);
    return 6 * term;
}
