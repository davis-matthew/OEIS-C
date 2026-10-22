/*
 * A323425  Number of ways n people in a line can each choose two others both
 *          on the same side of them.
 * https://oeis.org/A323425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (binomial(k-1,2)+binomial(n-k,2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t c1 = 0, c2 = 0;
        idx_t x = k - 1;
        if (x >= 2) {
            /* binomial(x,2) = x*(x-1)/2 */
            c1 = (seq_t)(x * (x - 1) / 2);
        }
        idx_t y = n - k;
        if (y >= 2) {
            /* binomial(y,2) = y*(y-1)/2 */
            c2 = (seq_t)(y * (y - 1) / 2);
        }
        seq_t term = c1 + c2;
        prod *= term;
        /* If term is zero the product becomes zero; break early for speed */
        if (prod == 0) break;
    }
    return prod;
}
