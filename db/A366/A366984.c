/*
 * A366984  a(n) = Sum_{k=1..n} binomial(k+2,2) * floor(n/k).
 * https://oeis.org/A366984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+2,2) * floor(n/k)
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t t  = (kk + 2) * (kk + 1) / 2;   /* binomial(k+2,2) */
        seq_t f  = (seq_t)(n / k);            /* floor(n/k) */
        total += t * f;
    }
    return total;
}
