/*
 * A246695  Row sums of the triangular array A246694.
 * https://oeis.org/A246695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*(n+1)*((-1)^n + 2*n^2 + 4*n + 7)
 * Range:     u8 n <= 9, u16 n <= 62, u32 n <= 1744, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;      /* cast index to seq_t once */
    seq_t n1  = nn + 1;        /* n + 1 */
    seq_t inner;

    /* Compute the inner part separately to avoid using a negative sign
       for odd n in an unsigned type.  For even n: 2n² + 4n + 8
       for odd  n: 2n² + 4n + 6                                 */
    if (nn & 1) {  /* odd n */
        inner = 2*nn*nn + 4*nn + 6;
    } else {       /* even n */
        inner = 2*nn*nn + 4*nn + 8;
    }

    return (n1 * inner) / 8;   /* division is exact by construction */
}
