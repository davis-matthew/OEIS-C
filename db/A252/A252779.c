/*
 * A252779  Number of ways of n-coloring the square grid graph G_(3,3) such
 *          that no rectangle exists having all 4 corners of the same color.
 * https://oeis.org/A252779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^2*(n^6+2*n^5+3*n^4-6*n^3-15*n^2-4*n+12)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices explicitly */
    if (n <= 1) {
        return 0;
    }

    /* Use seq_t for all intermediate calculations */
    seq_t n_seq = (seq_t)n;

    /* Compute powers of n up to n^6 */
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */
    seq_t n4 = n3 * n_seq;             /* n^4 */
    seq_t n5 = n4 * n_seq;             /* n^5 */
    seq_t n6 = n5 * n_seq;             /* n^6 */

    /* Compute the polynomial part: n^6+2*n^5+3*n^4-6*n^3-15*n^2-4*n+12 */
    seq_t pos = n6;
    pos += 2 * n5;        /* + 2*n^5 */
    pos += 3 * n4;        /* + 3*n^4 */
    pos += 12;            /* + 12 */

    seq_t neg = 6 * n3;   /* - 6*n^3 */
    neg += 15 * n2;       /* - 15*n^2 */
    neg += 4 * n_seq;     /* - 4*n */

    seq_t poly = pos - neg;   /* polynomial value (positive for n>=2) */

    /* Compute n*(n-1)^2 */
    seq_t nm1 = n_seq - 1;
    seq_t tmp = nm1 * nm1;
    tmp *= n_seq;            /* tmp = n*(n-1)^2 */

    /* Result */
    seq_t result = tmp * poly;
    return result;
}
