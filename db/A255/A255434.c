/*
 * A255434  Product_{k=0..n} (k^4+1).
 * https://oeis.org/A255434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0..n} (k^4 + 1) (product over 0 to n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;           /* running product */
    idx_t   k;

    for (k = (idx_t)0; k <= n; ++k)
    {
        seq_t k4 = (seq_t)k;          /* k */
        k4 = k4 * k4;                /* k^2 */
        k4 = k4 * k4;                /* k^4 */
        k4 = k4 + (seq_t)1;          /* k^4 + 1 */
        prod = prod * k4;            /* accumulate product */
    }

    return prod;
}
