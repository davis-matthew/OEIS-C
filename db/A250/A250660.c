/*
 * A250660  Number of (6+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*n^2 + 159*n + 128 (n ≥ 1)
 * Range:     u8 none, u16 n <= 42, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */
    seq_t nn = n;
    seq_t part1 = (seq_t)32 * nn * nn;
    seq_t part2 = (seq_t)159 * nn;
    return part1 + part2 + 128;
}
