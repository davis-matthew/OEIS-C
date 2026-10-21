/*
 * A254407  a(n) = n*(n+1)*(11*n +10)/6.
 * https://oeis.org/A254407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(11*n +10)/6
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 739, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t term1  = n_seq + 1;
    seq_t term2  = (seq_t)11 * n_seq + (seq_t)10;
    seq_t num    = n_seq * term1;
    num          = num * term2;
    return num / (seq_t)6;
}
