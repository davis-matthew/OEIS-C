/*
 * A396310  a(n)/9! is the coefficient of x^9 in the Taylor expansion of the
 *          k-th iteration of sin(x).
 * https://oeis.org/A396310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,1) + 1870*binomial(n,2) + 20244*binomial(n,3)
 *            + 29400*binomial(n,4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 0;

    /* binomial(n,1) = n */
    res += nn;

    /* binomial(n,2) = n*(n-1)/2  */
    if (n >= 2) {
        seq_t b2 = nn * (nn - 1) / 2;
        res += (seq_t)1870 * b2;
    }

    /* binomial(n,3) = n*(n-1)*(n-2)/6 */
    if (n >= 3) {
        seq_t b3 = nn * (nn - 1) * (nn - 2) / 6;
        res += (seq_t)20244 * b3;
    }

    /* binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24 */
    if (n >= 4) {
        seq_t b4 = nn * (nn - 1) * (nn - 2) * (nn - 3) / 24;
        res += (seq_t)29400 * b4;
    }

    return res;
}
