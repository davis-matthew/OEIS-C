/*
 * A268263  Number of length-(4+1) 0..n arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + n^4 + 4*n^3 + 3*n^2 + 6*n + 2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) = 17 */
    if (n == 0u) return 0; /* not used by the sequence */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */

    seq_t result = n5;                     /* n^5 */
    result += n4;                          /* + n^4 */
    result += 4 * n3;                      /* + 4*n^3 */
    result += 3 * n2;                      /* + 3*n^2 */
    result += 6 * nn;                      /* + 6*n */
    result += 2;                           /* + 2 */

    return result;
}
