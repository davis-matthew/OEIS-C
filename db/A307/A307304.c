/*
 * A307304  Number of inequivalent ways of placing 2 nonattacking rooks on n X
 *          n board up to rotations and reflections of the board.
 * https://oeis.org/A307304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/16)*n*(n^3-2*n^2+6*n-4) if n even * a(n) =
 *            (1/16)*(n-1)*(n^3-n^2+5*n-1) if n odd
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t result;

    if ((N & (seq_t)1) == (seq_t)0) {          /* even n */
        seq_t numerator = N * (n3 - (seq_t)2 * n2 + (seq_t)6 * N - (seq_t)4);
        result = numerator / (seq_t)16;
    } else {                                   /* odd n */
        seq_t temp = n3 - n2 + (seq_t)5 * N - (seq_t)1;
        seq_t numerator = (N - (seq_t)1) * temp;
        result = numerator / (seq_t)16;
    }
    return result;
}
