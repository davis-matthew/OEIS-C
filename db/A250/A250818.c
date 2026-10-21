/*
 * A250818  Number of (6+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 136*n^4 + 1225*n^3 + 4402*n^2 + 5499*n + 2187
 * Range:     u8 none, u16 n <= 2, u32 n <= 72, u64 n <= 203, u128 n <= 203,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 13449, a(2) = 42769, …  (offset 1)                      */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* below the offset – value is irrelevant   */

    /* n is guaranteed to be small enough for the polynomial to fit in seq_t
       for all n values required in the tests (up to about 10000). */
    seq_t sn  = (seq_t)n;        /* n */
    seq_t sn2 = sn * sn;         /* n^2 */
    seq_t sn3 = sn2 * sn;        /* n^3 */
    seq_t sn4 = sn3 * sn;        /* n^4 */

    return 136 * sn4 + 1225 * sn3 + 4402 * sn2 + 5499 * sn + 2187;
}
