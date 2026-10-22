/*
 * A379703  Antidiagonal sums of A343052.
 * https://oeis.org/A379703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(108 + 43*n + 8*n^2 + 3*(-1)^n*(4 + n))/48 for n >
 *            1 a(0) = a(1) = a(2) = 0
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First three terms are zero */
    if (n <= 2)
        return 0;

    /* Convert the index to seq_t for all arithmetic */
    seq_t nseq = n;            /* n */
    seq_t nsq  = nseq * nseq;  /* n^2 */
    seq_t part = 108 + 43 * nseq + 8 * nsq;  /* 108 + 43*n + 8*n^2 */
    seq_t base = nseq - 2;          /* n-2 */

    /* Compute the two components of the numerator */
    seq_t term1 = base * part;                        /* (n-2)*(108 + 43*n + 8*n^2) */
    seq_t term2 = 3 * base * (4 + nseq);              /* 3*(n-2)*(4+n) */

    /* Sign of the second part depends on parity of n */
    if (n & 1)            /* odd n -> subtract term2 */
        term1 -= term2;
    else                  /* even n -> add term2 */
        term1 += term2;

    /* Final division by 48 */
    return term1 / 48;
}
