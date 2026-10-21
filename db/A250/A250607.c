/*
 * A250607  Number of (n+1) X (4+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 100*2^(n-1) + 16*n^2 + 22*n - 18 (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^(n-1) iteratively. */
    seq_t pow = 1;
    for (idx_t i = 1; i < n; ++i)
    {
        pow *= (seq_t)2;
    }

    /* Convert index to seq_t for arithmetic. */
    seq_t mn = (seq_t)n;
    seq_t mn_sq = mn * mn;

    /* Final formula. */
    return (seq_t)100 * pow + (seq_t)16 * mn_sq + (seq_t)22 * mn - (seq_t)18;
}
