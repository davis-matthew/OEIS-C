/*
 * A356249  a(n) = Sum_{k=1..n} (k * floor(n/k))^3.
 * https://oeis.org/A356249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k*floor(n/k))^3
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 289, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m = n / k;
        seq_t km = (seq_t)k * (seq_t)m;   /* k * floor(n/k) */
        seq_t km2 = km * km;               /* (k * floor(n/k))^2 */
        seq_t km3 = km2 * km;              /* (k * floor(n/k))^3 */
        sum += km3;
    }
    return sum;
}
