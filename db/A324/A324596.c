/*
 * A324596  a(n) = n!^(3*n) * Product_{k=1..n} binomial(n + 1/k^2, n).
 * https://oeis.org/A324596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{j=1..n} product_{k=1..n} (j*k^2 + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j)
    {
        for (idx_t k = 1; k <= n; ++k)
        {
            seq_t term = (seq_t)j * (seq_t)k * (seq_t)k + (seq_t)1;
            res *= term;
        }
    }
    return res;
}
