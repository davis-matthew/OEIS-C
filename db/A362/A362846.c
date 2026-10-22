/*
 * A362846  a(n) = Sum_{k=0..n-1} Gamma(n + k) / Gamma(n - k) for n > 0, a(0)
 *          = 1. Row sums of A362787.
 * https://oeis.org/A362846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} Gamma(n + k) / Gamma(n - k) with a(0) = 1.
 *            Gamma(m) = (m‑1)! for integer m >= 1. For integer m,
 *            Gamma(n+k)/Gamma(n-k) = product_{j=n-k}^{n+k-1} j
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for arbitrary integer type seq_t. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 */

    seq_t sum = (seq_t)0;
    seq_t p   = (seq_t)1;              /* product for current k */

    for (idx_t k = 0; k < n; ++k) {
        sum += p;                       /* add term for k */

        if (k + 1 < n) {               /* prepare product for next k */
            p *= (seq_t)(n - k - 1);     /* multiply by n-k-1 */
            p *= (seq_t)(n + k);         /* multiply by n+k   */
        }
    }
    return sum;
}
