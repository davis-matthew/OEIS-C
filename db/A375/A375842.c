/*
 * A375842  a(n) = Product_{k=0..n} (k^5 + n).
 * https://oeis.org/A375842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^5 + n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 directly follows from the definition */
    if (n == 0)
        return (seq_t)0;

    seq_t acc = (seq_t)n;              /* factor for k = 0: 0^5 + n = n */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kseq = (seq_t)k;

        /* compute k^5 by successive multiplications */
        seq_t k2 = kseq * kseq;      /* k^2 */
        seq_t k3 = k2 * kseq;        /* k^3 */
        seq_t k4 = k3 * kseq;        /* k^4 */
        seq_t k5 = k4 * kseq;        /* k^5 */

        seq_t term = k5 + (seq_t)n;  /* k^5 + n */

        acc = acc * term;           /* accumulate the product */
    }
    return acc;
}
