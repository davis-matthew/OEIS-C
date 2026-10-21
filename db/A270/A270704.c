/*
 * A270704  Even 14-gonal (or tetradecagonal) numbers.
 * https://oeis.org/A270704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(12*n - 5)
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* cast index to seq_t */
    seq_t term = 12 * nn - (seq_t)5;/* 12*n - 5 */
    return (seq_t)2 * nn * term;    /* 2*n*(12*n - 5) */
}
