/*
 * A276940  a(1) = 2; for n > 1, a(n) = (n-2)! * n^3.
 * https://oeis.org/A276940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2; for n > 1, a(n) = (n-2)! * n^3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 8;

    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 2; ++i)
        fact *= (seq_t)i;

    seq_t ans = fact;
    ans *= (seq_t)n;
    ans *= (seq_t)n;
    ans *= (seq_t)n;
    return ans;
}
