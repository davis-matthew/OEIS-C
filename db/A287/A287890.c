/*
 * A287890  Number of unrooted labeled 4-cactus graphs on 3n+1 nodes.
 * https://oeis.org/A287890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)^(n-1) * (3*n)! / (2^n * n!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }

    /* pow_term = (3*n+1)^(n-1) */
    seq_t pow_term = big_pow((seq_t)(3*n + 1), (seq_t)(n - 1));
    seq_t result   = pow_term;

    /* We must divide the product \prod_{k=n+1}^{3n} k by 2^n.
       Count how many factors of 2 are still needed */
    idx_t need_twos = n;

    for (idx_t k = n + 1; k <= 3 * n; ++k) {
        idx_t kk = k;           /* copy of the current multiplier */

        if (need_twos > 0) {
            /* trailing zero count (number of factors 2 in k) */
#if defined(__GNUC__) || defined(__clang__)
            idx_t twos = __builtin_ctzll(kk);
#else
            idx_t twos = 0;
            idx_t tmp  = kk;
            while ((tmp & 1ULL) == 0) {
                ++twos;
                tmp >>= 1;
            }
#endif
            idx_t take = (twos < need_twos) ? twos : need_twos;
            kk      >>= take;   /* remove the used powers of two */
            need_twos -= take;
        }

        result *= kk;
    }

    return result;
}
