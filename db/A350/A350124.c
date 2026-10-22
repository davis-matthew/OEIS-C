/*
 * A350124  a(n) = Sum_{k=1..n} k^2 * floor(n/k)^3.
 * https://oeis.org/A350124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^2 * floor(n/k)^3
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 877, u64 n <= 10000,
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

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t q = n / k;

        /* compute k^2 in seq_t */
        seq_t kk = (seq_t)k * k;

        /* compute q^3 in seq_t, avoiding intermediate overflow
           by doing the multiplications sequentially */
        seq_t q_sq = (seq_t)q * q;        /* q^2 */
        seq_t q_cu = q_sq * q;           /* q^3 */

        seq_t term = kk * q_cu;
        sum += term;
    }

    return sum;
}
