/*
 * A356768  a(n) = (n^2+n+1)*(n^2+n)*n^2.
 * https://oeis.org/A356768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n + 1)*(n^2 + n)*n^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n2_plus_n = n2 + n_seq;
    seq_t n2_plus_n_plus1 = n2_plus_n + (seq_t)1;
    return n2 * n2_plus_n * n2_plus_n_plus1;
}
