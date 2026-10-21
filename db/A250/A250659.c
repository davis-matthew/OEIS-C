/*
 * A250659  Number of (5+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*n^2 + 79*n + 64 (for n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 61, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;              /* convert the index to seq_t */
    return 16 * x * x + 79 * x + 64; /* exact polynomial evaluation */
}
