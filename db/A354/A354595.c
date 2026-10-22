/*
 * A354595  a(n) = n^2 + 4*floor(n/2)^2.
 * https://oeis.org/A354595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 4*floor(n/2)^2
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t  = (seq_t)n;       /* cast index to the sequence type */
    seq_t half = n_t >> 1;       /* floor(n/2) using integer shift */
    return n_t * n_t + (seq_t)4 * half * half;
}
