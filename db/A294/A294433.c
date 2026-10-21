/*
 * A294433  Expansion of (1+11*x+24*x^2+11*x^3+x^4)/(1-x)^5.
 * https://oeis.org/A294433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 7*n/2 + 11*n^2/2 + 4*n^3 + 2*n^4
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 213, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = (seq_t)n;          /* n */
    seq_t t2 = t * t;             /* n^2 */
    seq_t t3 = t * t2;            /* n^3 */
    seq_t t4 = t2 * t2;           /* n^4 */

    seq_t term5 = 4 * t4;         /* 2*n^4  * 2 = 4*n^4 */
    seq_t term4 = 8 * t3;         /* 4*n^3  * 2 = 8*n^3 */
    seq_t term3 = 11 * t2;        /* 11*n^2 */
    seq_t term2 = 7 * t;          /* 7*n   */
    /* constant 2 already included */

    seq_t sum = term5 + term4 + term3 + term2 + 2;
    return sum / 2;               /* divide numerator by 2 */
}
