/*
 * A273658  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, eight of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273658
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,8) * Σ_{j=1..8} Stirling2(8,j) * j^(n-8)
 * Range:     u8 none, u16 n <= 8, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return 0; /* undefined below offset */

    /* Stirling numbers of the second kind S(8,j) for j = 0..8 */
    static const uint64_t S_vals[9] = {
        0ULL, 1ULL, 127ULL, 966ULL, 1701ULL,
        1050ULL, 266ULL, 28ULL, 1ULL
    };

    seq_t binom = binomial(n, 8);   /* C(n,8) */
    seq_t sum   = 0;
    idx_t exp   = n - 8;

    for (idx_t j = 1; j <= 8; ++j) {
        seq_t pow_val = pow_uint((uint64_t)j, exp);   /* j^(n-8) */
        seq_t sj      = (seq_t)S_vals[j];            /* Stirling2(8,j) */
        sum += pow_val * sj;
    }

    return binom * sum;
}
