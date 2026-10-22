/*
 * A323544  a(n) = Product_{k=0..n} (k^6 + (n-k)^6).
 * https://oeis.org/A323544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0}^{n} (k^6 + (n-k)^6)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 yields 0 as defined in the sequence. */
    if (n == 0)
        return 0;

    seq_t prod = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute k^6 */
        seq_t kk = (seq_t)k;
        seq_t kk2 = kk * kk;        /* k^2   */
        seq_t kk3 = kk2 * kk;       /* k^3   */
        seq_t kk6 = kk3 * kk3;       /* k^6   */

        /* Compute (n-k)^6 */
        seq_t nk = (seq_t)(n - k);
        seq_t nk2 = nk * nk;        /* (n-k)^2 */
        seq_t nk3 = nk2 * nk;       /* (n-k)^3 */
        seq_t nk6 = nk3 * nk3;        /* (n-k)^6 */

        seq_t term = kk6 + nk6;     /* k^6 + (n-k)^6 */
        prod = prod * term;         /* multiply into product */
    }

    return prod;
}
