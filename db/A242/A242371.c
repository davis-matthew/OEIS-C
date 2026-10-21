/*
 * A242371  Modified eccentric connectivity index of the cycle graph with n
 *          vertices, C[n].
 * https://oeis.org/A242371
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(n-1) if n is odd; a(n) = 2*n^2 if n is even (n ≥ 3)
 * Range:     u8 n <= 11, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if ((nseq & 1U) == 1U)          /* odd n */
        return 2 * nseq * (nseq - 1);
    else                            /* even n */
        return 2 * nseq * nseq;
}
