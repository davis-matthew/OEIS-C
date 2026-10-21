/*
 * A282612  Number of inequivalent 3 X 3 matrices with entries in {1,2,3,..,n}
 *          up to row permutations.
 * https://oeis.org/A282612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n^3+2)*(n+1)*(n^2-n+1)/6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 168, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* promote the index to the result type */
    seq_t sn = (seq_t)n;

    /* n^3 */
    seq_t n3 = sn;
    n3 *= sn;   /* n^2 */
    n3 *= sn;   /* n^3 */

    /* the four factors of the numerator */
    seq_t x1 = n3;                             /* n^3           */
    seq_t x2 = n3 + (seq_t)2;                 /* n^3 + 2       */
    seq_t x3 = sn + (seq_t)1;                 /* n + 1         */
    seq_t tmp = sn * sn;                      /* n^2           */
    seq_t x4 = tmp - sn + (seq_t)1;           /* n^2 - n + 1   */

    /* cancel the factor 2 – always present in the product */
    if ((x1 & (seq_t)1) == 0)
        x1 /= (seq_t)2;
    else if ((x2 & (seq_t)1) == 0)
        x2 /= (seq_t)2;
    else if ((x3 & (seq_t)1) == 0)
        x3 /= (seq_t)2;
    else
        x4 /= (seq_t)2;

    /* cancel the factor 3 – always present in the product */
    if (x1 % (seq_t)3 == 0)
        x1 /= (seq_t)3;
    else if (x2 % (seq_t)3 == 0)
        x2 /= (seq_t)3;
    else if (x3 % (seq_t)3 == 0)
        x3 /= (seq_t)3;
    else
        x4 /= (seq_t)3;

    /* multiply the remaining factors – all divisions already performed */
    seq_t result = x1;
    result *= x2;
    result *= x3;
    result *= x4;

    return result;
}
