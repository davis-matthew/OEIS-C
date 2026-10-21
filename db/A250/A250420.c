/*
 * A250420  Number of length 3+1 0..n arrays with the sum of the minimum of
 *          each adjacent pair multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A250420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13/6)*n^3 + (13/4)*n^2 + (10/3)*n + 1 if n is even a(n)
 *            = (13/6)*n^3 + (13/4)*n^2 + (10/3)*n + 5/4 if n is odd The above
 *            can be rewritten as a(n) = (26*n^3 + 39*n^2 + 40*n + 12)/12 (n
 *            even) a(n) = (26*n^3 + 39*n^2 + 40*n + 15)/12 (n odd) which
 *            yields integer results for all n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic in the target type */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;   /* n^2 */
    seq_t n3 = n2 * nn;   /* n^3 */

    seq_t numerator;
    if ((nn & (seq_t)1) == 0) {          /* n even */
        numerator = (seq_t)26 * n3
                   + (seq_t)39 * n2
                   + (seq_t)40 * nn
                   + (seq_t)12;
    } else {                              /* n odd */
        numerator = (seq_t)26 * n3
                   + (seq_t)39 * n2
                   + (seq_t)40 * nn
                   + (seq_t)15;
    }
    return numerator / (seq_t)12;
}
