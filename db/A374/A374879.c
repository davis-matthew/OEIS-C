/*
 * A374879  Obverse convolution (n)**(floor(3n/2)); see Comments.
 * https://oeis.org/A374879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; For n > 0: let k = n/2; if n is even: a(n) =
 *            (∏_{i=n}^{3k-1} i)^2 * (3k); if n is odd : a(n) =
 *            (∏_{i=n}^{3k+1} i)^2;
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    if (n & 1u) {                     /* odd n = 2k+1 */
        idx_t k = (n - 1u) / 2u;       /* k = (n-1)/2 */
        seq_t prod = (seq_t)1;
        idx_t upper = n + k;           /* upper = 3k+1 */
        for (idx_t j = n; j <= upper; ++j)
            prod *= (seq_t)j;
        return prod * prod;
    } else {                           /* even n = 2k */
        idx_t k = n / 2u;              /* k = n/2 */
        seq_t prod = (seq_t)1;
        idx_t upper = n + k - 1u;       /* upper = 3k-1 */
        for (idx_t j = n; j <= upper; ++j)
            prod *= (seq_t)j;
        seq_t res = prod * prod * (seq_t)(3u * k);
        return res;
    }
}
