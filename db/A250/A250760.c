/*
 * A250760  Number of (5+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1697/2)*n^2 + (3149/2)*n + 729
 * Range:     u8 none, u16 n <= 5, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t val = ((seq_t)1697) * nn * nn
              + ((seq_t)3149) * nn
              + ((seq_t)1458);
    return val / 2;
}
