/*
 * A305991  Expansion of (1-27*x)^(1/9).
 * https://oeis.org/A305991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*(9*n-10)/n * a(n-1) with a(0)=1 * (recurrence derived
 *            from n*a(n)+3*(-9*n+10)a(n-1)=0)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t k = (seq_t)i;
        seq_t factor = 3 * ((seq_t)9 * k - 10);
        res = (res * factor) / k;
    }
    return res;
}
