/*
 * A250816  Number of (4+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*n^4 + 121*n^3 + 439*n^2 + 573*n + 243
 * Range:     u8 none, u16 n <= 6, u32 n <= 132, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term4 = nn * nn * nn * nn * (seq_t)13;
    seq_t term3 = nn * nn * nn * (seq_t)121;
    seq_t term2 = nn * nn * (seq_t)439;
    seq_t term1 = nn * (seq_t)573;
    return term4 + term3 + term2 + term1 + (seq_t)243;
}
