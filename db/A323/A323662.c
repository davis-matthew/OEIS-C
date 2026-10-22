/*
 * A323662  a(n) = Product_{k=0..n} (k^13 + (n-k)^13).
 * https://oeis.org/A323662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^13 + (n-k)^13)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute k^13 */
        seq_t k_pow = 1;
        for (int i = 0; i < 13; ++i)
            k_pow *= (seq_t)k;

        /* compute (n-k)^13 */
        idx_t nk = n - k;
        seq_t nk_pow = 1;
        for (int i = 0; i < 13; ++i)
            nk_pow *= (seq_t)nk;

        /* accumulate product */
        result *= (k_pow + nk_pow);
    }
    return result;
}
