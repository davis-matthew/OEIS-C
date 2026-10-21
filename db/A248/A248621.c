/*
 * A248621  Floor of sums of the squares of the non-integer cube roots of n,
 *          as partitioned by the integer roots:
 *          floor(Sum_{j=n^3+1..(n+1)^3-1} j^(2/3)).
 * https://oeis.org/A248621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^4 + 6*n^3 + 5*n^2 + 2*n
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 194, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    return 3 * n4 + 6 * n3 + 5 * n2 + 2 * nn;
}
