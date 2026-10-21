/*
 * A241452  a(n) = pg(3, n) + pg(4, n) + ... + pg(n, n) where pg(m, n) is the
 *          n-th m-th-order polygonal number.
 * https://oeis.org/A241452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*(-10 + 9*n - 4*n^2 + n^3)/4 for n > 1;
 * a(0) = 0, a(1) = 0
 */
seq_t a(idx_t n)
{
    /* handle the offset */
    if (n <= (idx_t)1)
        return 0;

    /* Cast index to the sequence type */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    seq_t n4 = n2 * n2;          /* n^4 */

    /* Compute the numerator: n^4 - 4*n^3 + 9*n^2 - 10*n */
    seq_t num = n4 - (seq_t)4 * n3 + (seq_t)9 * n2 - (seq_t)10 * ns;

    /* Divide by 4 */
    return num / (seq_t)4;
}
