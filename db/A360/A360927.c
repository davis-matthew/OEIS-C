/*
 * A360927  Expansion of the g.f. x*(1 + 3*x + 4*x^2 + 4*x^3)/((1 - x)^2*(1 +
 *          x)).
 * https://oeis.org/A360927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 a(1) = 1 a(n) = 6*n - 8 if n is even and n >= 2 a(n) =
 *            6*n - 9 if n is odd and n >= 3 (This equivalently satisfies the
 *            recurrence a(n)=a(n-1)+a(n-2)-a(n-3) for n>4.)
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0U;
    if (n == 1)
        return (seq_t)1U;

    seq_t val = (seq_t)n;
    /* n & 1 gives 0 for even, 1 for odd */
    return (seq_t)6 * val - ((n & 1U) ? (seq_t)9 : (seq_t)8);
}
