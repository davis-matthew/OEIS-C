/*
 * A324213  Number of k with 0 <= k <= sigma(n) such that n-k and 2n-sigma(n)
 *          are relatively prime.
 * https://oeis.org/A324213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..sigma(n)} [gcd(n-i, n-(sigma(n)-i)) == 1]
 * Range:     u8 n <= 127, u16 n <= 9962, u32 n <= 9962, u64 n <= 9962,
 *            u128 n <= 9962, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    seq_t sigma = sigma_of(n);                     /* σ(n) */
    idx_t sigma_idx = (idx_t)sigma;                /* loop bound */
    seq_t count   = 0;

    for (idx_t i = 0; i <= sigma_idx; ++i) {
        /* |n - i|  */
        seq_t a_abs = (i > n) ? (seq_t)(i - n) : (seq_t)(n - i);

        /* |n - σ(n) + i|  =  |(σ(n) - i) - n|  */
        seq_t diff = sigma - (seq_t)i;              /* σ(n) - i, always ≥ 0 */
        seq_t b_abs = (diff > (seq_t)n) ? diff - (seq_t)n : (seq_t)n - diff;

        if (gcd(a_abs, b_abs) == 1)
            ++count;
    }
    return count;
}
