/*
 * A277354  a(n) = Product_{k=1..n} (4*k^2+1).
 * https://oeis.org/A277354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (4*k^2 + 1) (a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;   /* k^2 */
        seq_t term = 4 * kk + 1;          /* 4*k^2 + 1 */
        prod = prod * term;               /* multiply into product */
    }
    return prod;
}
