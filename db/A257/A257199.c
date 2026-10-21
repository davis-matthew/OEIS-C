/*
 * A257199  a(n) = n*(n+1)*(n+2)*(n^2+2*n+17)/120.
 * https://oeis.org/A257199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n^2 + 2*n + 17)/120
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nseq = (seq_t)n;

    /* Compute the four factors */
    seq_t p0 = nseq;                       /* n           */
    seq_t p1 = nseq + 1;                   /* n+1         */
    seq_t p2 = nseq + 2;                   /* n+2         */
    seq_t p3 = nseq * nseq + (nseq << 1) + 17; /* n^2 + 2n + 17 */

    /* Denominator 120 = 2^3 * 3 * 5 – strip the prime factors from the parts */
    const unsigned int factors[5] = {2, 2, 2, 3, 5};
    for (int fidx = 0; fidx < 5; ++fidx) {
        unsigned int f = factors[fidx];
        if (p0 % f == 0) {
            p0 /= f;
        } else if (p1 % f == 0) {
            p1 /= f;
        } else if (p2 % f == 0) {
            p2 /= f;
        } else {
            p3 /= f;
        }
    }

    /* Multiply the reduced factors to get the final result */
    seq_t result = p0;
    result *= p1;
    result *= p2;
    result *= p3;
    return result;
}
