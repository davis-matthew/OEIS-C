/*
 * A375844  a(n) = Product_{k=0..n} (k^7 + n).
 * https://oeis.org/A375844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^7 + n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t k_s = (seq_t)k;
        seq_t power = k_s;
        for (int i = 0; i < 6; ++i)   /* compute k^7 */
            power *= k_s;
        seq_t term = power + (seq_t)n;
        prod *= term;
    }
    return prod;
}
