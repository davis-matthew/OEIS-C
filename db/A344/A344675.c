/*
 * A344675  a(n) = Sum_{k=1..n} floor(n^3/k^3).
 * https://oeis.org/A344675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n^3/k^3)
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1528, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;
        sum += n3 / k3;
    }
    return sum;
}
