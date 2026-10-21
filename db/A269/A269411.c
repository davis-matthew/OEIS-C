/*
 * A269411  Number of length-5 0..n arrays with no repeated value greater than
 *          or equal to the previous repeated value.
 * https://oeis.org/A269411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + (11/2)*n^3 + n^2 - (1/2)*n
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 80, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset: 1 – a(1) = 12 */
    if (n == 0)
        return 0; /* value not used, but keeps type safety */

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;        /* n^2 */
    seq_t n3  = n2 * nn;        /* n^3 */
    seq_t n4  = n3 * nn;        /* n^4 */
    seq_t n5  = n4 * nn;        /* n^5 */

    /* Compute 2*a(n) as integer: 2*n^5 + 10*n^4 + 11*n^3 + 2*n^2 - n */
    seq_t two_a = 2 * n5 + 10 * n4 + 11 * n3 + 2 * n2 - nn;

    return two_a / 2;
}
