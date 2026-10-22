/*
 * A389545  a(n) = Sum_{k=0..4} 2^k * binomial(n,k).
 * https://oeis.org/A389545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..4} 2^k * binomial(n,k)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = 0;
    seq_t nn = (seq_t)n;

    /* k = 0 : 2^0 * C(n,0) = 1 */
    s += (seq_t)1;

    /* k = 1 : 2^1 * C(n,1) = 2 * n */
    if (n >= 1)
        s += (seq_t)2 * nn;

    /* k = 2 : 2^2 * C(n,2) = 4 * n*(n-1)/2 = 2 * n*(n-1) */
    if (n >= 2) {
        seq_t t = nn * (nn - (seq_t)1);
        t = t * (seq_t)2;
        s += t;
    }

    /* k = 3 : 2^3 * C(n,3) = 8 * n*(n-1)*(n-2)/6 = 4 * n*(n-1)*(n-2)/3 */
    if (n >= 3) {
        seq_t t = nn * (nn - (seq_t)1) * (nn - (seq_t)2);
        t = t * (seq_t)4;
        s += t / (seq_t)3;
    }

    /* k = 4 : 2^4 * C(n,4) = 16 * n*(n-1)*(n-2)*(n-3)/24 = 2 * n*(n-1)*(n-2)*(n-3)/3 */
    if (n >= 4) {
        seq_t t = nn * (nn - (seq_t)1) * (nn - (seq_t)2) * (nn - (seq_t)3);
        t = t * (seq_t)2;
        s += t / (seq_t)3;
    }

    return s;
}
