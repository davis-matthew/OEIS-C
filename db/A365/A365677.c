/*
 * A365677  Number of increasing geometric progressions in {1,2,3,...,n} with
 *          rational ratio and length >= 3.
 * https://oeis.org/A365677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=3}^{1+⌊log₂ n⌋} Σ_{p=2}^{⌊n^{1/(k-1)}⌋} φ(p) · ⌊n /
 *            p^{k-1}⌋ where φ is Euler's totient function
 * Range:     u8 n <= 168, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_val.h"

seq_t a(idx_t n)
{
    seq_t n_seq = n;
    seq_t result = 0;

    if (n < 3) return 0;                /* early exit for n = 1,2 */

    /* compute k_max = 1 + floor(log₂ n) without floating point */
    idx_t k_max = 1;
    idx_t pw = 2;                        /* 2^1 */
    while (pw <= n) {
        ++k_max;
        pw <<= 1;
    }

    for (idx_t k = 3; k <= k_max; ++k) {
        idx_t exp = k - 1;

        for (idx_t p = 2; ; ++p) {
            /* compute p_pow = p^exp with overflow guard */
            seq_t p_pow = 1;
            int overflowed = 0;
            for (idx_t e = 0; e < exp; ++e) {
                if (p_pow > n_seq / p) {          /* would overflow or exceed n */
                    p_pow = n_seq + 1;
                    overflowed = 1;
                    break;
                }
                p_pow *= p;
            }
            if (overflowed || p_pow > n_seq) break;   /* p^exp > n */

            seq_t phi = phi_val(p);
            seq_t term = phi * (n_seq / p_pow);
            result += term;
        }
    }

    return result;
}
