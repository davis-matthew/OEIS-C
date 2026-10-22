/*
 * A359043  Sum of adjusted partial sums of the n-th composition in standard
 *          order (A066099). Row sums of A242628.
 * https://oeis.org/A359043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=0}^{b-1} p_i*(i+1) – b*(b-1)/2 where: - b is the
 *            number of 1‑bits in n (popcount(n)), - pos[0..b-1] are the bit
 *            indices of those 1‑bits (LSB = 0), - p_0 = pos[0] + 1, - p_i =
 *            pos[i] – pos[i-1] for i ≥ 1 This computes the sum of adjusted
 *            partial sums of the composition corresponding to...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Record the positions of set bits (LSB = 0). */
    idx_t pos[64];
    size_t b = 0;
    idx_t k = n;
    idx_t bit = 0;
    while (k)
    {
        if (k & (idx_t)1)
            pos[b++] = bit;
        k >>= 1;
        bit++;
    }

    /* Compute weighted sum ∑ p_i*(i+1). */
    seq_t wsum = 0;
    for (size_t i = 0; i < b; ++i)
    {
        seq_t part;
        if (i == 0)
            part = (seq_t)pos[0] + 1;
        else
            part = (seq_t)(pos[i] - pos[i - 1]);

        wsum += part * ((seq_t)i + 1);
    }

    seq_t bb = (seq_t)b;
    seq_t result = wsum - bb * (bb - 1) / 2;
    return result;
}
