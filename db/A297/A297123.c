/*
 * A297123  a(n) = 462*n^6 + 330*n^4 - 165*n^3 + 55*n^2 - 11*n + 1.
 * https://oeis.org/A297123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 462*n^6 + 330*n^4 - 165*n^3 + 55*n^2 - 11*n + 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 14, u64 n <= 584,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the sequence type */
    seq_t x = (seq_t)n;

    /* compute successive powers of n */
    seq_t n2 = x * x;       /* n^2 */
    seq_t n3 = n2 * x;      /* n^3 */
    seq_t n4 = n3 * x;      /* n^4 */
    seq_t n6 = n4 * n2;     /* n^6 (= n^4 * n^2) */

    /* evaluate the polynomial */
    seq_t res = (seq_t)462 * n6;          /* 462*n^6   */
    res += (seq_t)330 * n4;              /* +330*n^4   */
    seq_t p3 = (seq_t)165 * n3;          /* 165*n^3   */
    res -= p3;                           /* -165*n^3  */
    seq_t p2 = (seq_t)55 * n2;           /* 55*n^2    */
    res += p2;                           /* +55*n^2   */
    seq_t p1 = (seq_t)11 * x;            /* 11*n      */
    res -= p1;                           /* -11*n     */
    res += (seq_t)1;                     /* +1         */

    return res;
}
