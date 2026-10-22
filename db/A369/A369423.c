/*
 * A369423  Number of 3 X 3 Fishburn matrices with entries in the set
 *          {0,1,...,n}.
 * https://oeis.org/A369423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n+1)*(n^2+3*n+1) = n^6 + 4*n^5 + 4*n^4 + n^3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 1624,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */
    seq_t n4 = n3 * nn;       /* n^4 */
    seq_t n5 = n4 * nn;       /* n^5 */
    seq_t n6 = n5 * nn;       /* n^6 */
    seq_t term = n6 + (seq_t)4 * n5 + (seq_t)4 * n4 + n3;
    return term;
}
