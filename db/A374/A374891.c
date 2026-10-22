/*
 * A374891  Obverse convolution (1)**A000984; see Comments.
 * https://oeis.org/A374891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (binom(2k,k)+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t bin  = 1;         /* binom(0,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* multiply by (C(2k,k) + 1) */
        prod *= bin + 1;

        /* update bin to C(2(k+1),(k+1)) for next iteration */
        if (k != n)
        {
            seq_t num = bin * (seq_t)(4 * (k + 1) - 2);
            bin = num / (seq_t)(k + 1);
        }
    }

    return prod;
}
