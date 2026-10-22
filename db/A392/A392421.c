/*
 * A392421  Number of mutual-visibility sets of size 3 in the n-cycle graph.
 * https://oeis.org/A392421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n-1)/24 for odd n
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            a(n) = n*(n+8)*(n-2)/24 for even n   */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* convert to seq_t once */
    seq_t tn = (seq_t)n;

    if (n & 1) /* odd */
    {
        seq_t term = tn * (seq_t)(n + 1) * (seq_t)(n - 1);
        return term / 24;
    }
    else /* even */
    {
        seq_t term = tn * (seq_t)(n + 8) * (seq_t)(n - 2);
        return term / 24;
    }
}
