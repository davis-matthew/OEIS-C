/*
 * A344816  a(n) = Sum_{k=1..n} floor(n/k) * 5^(k-1).
 * https://oeis.org/A344816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * 5^(k-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t result = 0;
    seq_t pow     = 1;   /* pow = 5^(k-1) for current k */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t mult = (seq_t)(n / k);
        result += pow * mult;
        pow *= (seq_t)5;
    }

    return result;
}
