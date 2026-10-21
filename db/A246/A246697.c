/*
 * A246697  Row sums of the triangular array A246696.
 * https://oeis.org/A246697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 6*n^2 + 9*n + 4 + n*(-1)^n) / 4
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;                 /* n^2   */
    seq_t n3 = n2 * nseq;                   /* n^3   */

    seq_t term1 = (seq_t)2 * n3;            /* 2*n^3 */
    seq_t term2 = (seq_t)6 * n2;            /* 6*n^2 */
    seq_t term3 = (seq_t)9 * nseq;          /* 9*n   */

    /* Numerator of the formula */
    seq_t sum = term1 + term2 + term3 + (seq_t)4; /* base 2n^3+6n^2+9n+4 */

    /* Add n*(-1)^n: +n for even n, -n for odd n */
    if (n & 1) {
        sum -= nseq;                        /* odd: subtract n */
    } else {
        sum += nseq;                        /* even: add n    */
    }

    return sum / (seq_t)4;                  /* divide by 4 */
}
