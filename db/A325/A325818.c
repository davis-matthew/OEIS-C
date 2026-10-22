/*
 * A325818  a(n) is the largest k <= sigma(n) such that n-k and n-(sigma(n)-k)
 *          are relatively prime.
 * https://oeis.org/A325818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k ≤ σ(n) | gcd(n−k , n−σ(n)+k) = 1}
 * Range:     u8 n <= 107, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset 1 – a(1) is the first term */

    seq_t sigma = sigma_of(n);
    seq_t   k     = sigma;            /* candidate k  */
    idx_t   n_idx  = n;
    idx_t   s_idx  = (idx_t)sigma;    /* σ(n) as idx_t for arithmetic */

    for ( ; k > 0; --k )
    {
        idx_t k_idx = (idx_t)k;

        /* |n - k|  */
        idx_t d1_idx = (n_idx >= k_idx) ? n_idx - k_idx : k_idx - n_idx;
        seq_t d1     = (seq_t)d1_idx;

        /* |n + k - σ(n)|  */
        idx_t t_idx = n_idx + k_idx;                     /* n + k   */
        idx_t d2_idx = (t_idx >= s_idx) ? t_idx - s_idx : s_idx - t_idx;
        seq_t d2     = (seq_t)d2_idx;

        if (gcd(d1, d2) == 1)
            return k;            /* largest valid k found */
    }
    return 0;   /* unreachable for n ≥ 1 */
}
