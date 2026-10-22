/*
 * A393462  Wiener index of the n-necklace graph.
 * https://oeis.org/A393462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1+(-1)^n+12*n^2)/2
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 768, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* compute the inner term: (1+(-1)^n)+12*n^2
       (1+(-1)^n) simplifies to 2 when n is even, 0 when odd */
    seq_t inner = ((n & 1) == 0 ? (seq_t)2 : (seq_t)0) +
                  (seq_t)12 * n_seq * n_seq;

    /* overall formula: n * inner / 2 */
    return (n_seq * inner) / (seq_t)2;
}
