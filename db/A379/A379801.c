/*
 * A379801  Nearest integer to n*(1+sqrt(6))/2.
 * https://oeis.org/A379801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*(1+sqrt(6))/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 5557, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* floor(n * sqrt(6)) = floor( sqrt(6 * n^2) ) */
    seq_t m = n_seq * n_seq;
    m = m * (seq_t)6;
    seq_t s = isqrt_seq(m);
    /* a(n) = (n + floor(n*sqrt(6)) + 1) / 2  */
    return (n_seq + s + 1) / (seq_t)2;
}
