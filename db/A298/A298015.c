/*
 * A298015  Coordination sequence of snub-632 tiling with respect to a
 *          trivalent node of type short-short-short.
 * https://oeis.org/A298015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * if n < 6: {1,3,6,15,21,18}[n] * else: let k =
 *            floor(n/3) * if n%3==0: 18*k-3 * if n%3==1: 24*k * if n%3==2:
 *            12*k+6
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)21;
    if (n == 5) return (seq_t)18;

    seq_t k = (seq_t)n / (seq_t)3;
    seq_t r = (seq_t)n % (seq_t)3;
    if (r == 0)
        return (seq_t)18 * k - (seq_t)3;
    else if (r == 1)
        return (seq_t)24 * k;
    else /* r == 2 */
        return (seq_t)12 * k + (seq_t)6;
}
