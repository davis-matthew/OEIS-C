/*
 * A339918  a(n) = Sum_{k=1..n} floor(3*n/k).
 * https://oeis.org/A339918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(3*n/k)
 * Range:     u8 n <= 23, u16 n <= 2633, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sum = (seq_t)0;
    seq_t t = (seq_t)3 * (seq_t)n;   /* 3*n stored in seq_t */
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        sum += t / (seq_t)k;
    }
    return sum;
}
