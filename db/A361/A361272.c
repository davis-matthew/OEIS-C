/*
 * A361272  Number of 1243-avoiding even Grassmannian permutations of size n.
 * https://oeis.org/A361272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (57 - 9*(-1)^n - 28*n + 6*n^2 + 4*n^3)/48
 * Range:     u8 n <= 14, u16 n <= 91, u32 n <= 3720, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long nn = (long long)n;
    long long sign = (nn & 1LL) ? -1LL : 1LL;
    long long num = 57 - 9 * sign - 28 * nn + 6 * nn * nn + 4 * nn * nn * nn;
    return (seq_t)(num / 48);
}
