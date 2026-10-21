/*
 * A262011  a(n) = (1/n!) * Product_{k=1..n} (k^3 + 1).
 * https://oeis.org/A262011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Product_{k=1..n} (k^2 - k + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk   = (seq_t)k * k;            /* k^2   */
        seq_t term = kk - (seq_t)k + (seq_t)1;/* k^2 - k + 1 */
        prod *= term;                        /* accumulate product */
    }

    prod *= (seq_t)(n + 1);                  /* multiply by (n+1) */
    return prod;
}
