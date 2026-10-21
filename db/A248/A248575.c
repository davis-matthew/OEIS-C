/*
 * A248575  Rounded sums of the non-integer cube roots of n, as partitioned by
 *          the integer roots: round(Sum_{j=n^3+1..(n+1)^3-1} j^(1/3)).
 * https://oeis.org/A248575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1-(-1)^n+8*n+18*n^2+12*n^3)/4
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 726, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* parity term: 0 if n even, 2 if n odd */
    seq_t parity = (n & (idx_t)1) ? (seq_t)2 : (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;                 /* n^2 */
    seq_t n3 = n2 * n_seq;                    /* n^3 */

    seq_t numerator = parity
                     + (seq_t)8 * n_seq
                     + (seq_t)18 * n2
                     + (seq_t)12 * n3;

    return numerator / (seq_t)4;
}
