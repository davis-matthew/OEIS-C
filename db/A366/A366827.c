/*
 * A366827  -a(n)/7! is the coefficient of x^7 in the Taylor expansion of the
 *          k-th iteration of sin(x).
 * https://oeis.org/A366827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,1) + 126*binomial(n,2) + 350*binomial(n,3)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 418, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = 175 * nn * nn;          /* 175*n^2 */
    t = t - 336 * nn + 164;           /* 175*n^2 - 336*n + 164 */
    return t * nn / 3;
}
