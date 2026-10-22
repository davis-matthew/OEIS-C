/*
 * A323543  a(n) = Product_{k=0..n} (k^5 + (n-k)^5).
 * https://oeis.org/A323543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^5 + (n-k)^5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow5(seq_t x)
{
    seq_t y = x * x;   /* x^2 */
    y *= x;            /* x^3 */
    y *= x;            /* x^4 */
    y *= x;            /* x^5 */
    return y;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t k5 = pow5((seq_t)k);
        idx_t nk = n - k;
        seq_t n5 = pow5((seq_t)nk);

        seq_t term = k5 + n5;
        prod *= term;
    }
    return prod;
}
