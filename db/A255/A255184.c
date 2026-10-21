/*
 * A255184  25-gonal numbers: a(n) = n*(23*n-21)/2.
 * https://oeis.org/A255184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(23*n-21)/2
 * Range:     u8 n <= 5, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t rn = (seq_t)n;
    /* Compute the second factor: 23*n - 21 */
    seq_t t = rn * (seq_t)23 - (seq_t)21;

    /* Divide the even factor first to keep intermediate values small */
    if ((rn & (seq_t)1) == 0) {
        /* rn is even */
        return (rn / (seq_t)2) * t;
    } else {
        /* t is even */
        return rn * (t / (seq_t)2);
    }
}
