/*
 * A308217  a(n) is the multiplicative inverse of A001844(n) modulo
 *          A001844(n+1); where A001844 is the sequence of centered square
 *          numbers.
 * https://oeis.org/A308217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n/2 if n is even * a(n) = n^2 + (7*(n+1))/2 if n is
 *            odd * * This matches the conjectured closed form for A308217
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence integer type. */
    seq_t sn = (seq_t)n;

    if ((n & 1U) == 0) {           /* even n */
        return 1 + sn / 2;         /* 1 + n/2  */
    } else {                       /* odd n */
        seq_t sq = sn * sn;        /* n^2      */
        seq_t inc = ((sn + 1) * 7) / 2; /* (7*(n+1))/2 */
        return sq + inc;           /* n^2 + (7*(n+1))/2 */
    }
}
