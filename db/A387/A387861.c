/*
 * A387861  Number of black-white-balanced permutations of [n].
 * https://oeis.org/A387861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n≡1,2 (mod 4); otherwise * a(n) = ⌊n/2⌋! * ⌈n/2⌉! *
 *            binom(⌈n/2⌉, ⌈n/2⌉/2) * binom(⌊n/2⌋, ⌈n/2⌉/2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 28,
 *            big any n
 * Helpers:   _fact_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_fact_n.h"

static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t mod4 = n & 3;           /* n % 4 */
    if (mod4 == 1 || mod4 == 2)
        return 0;

    idx_t k = n >> 1;             /* floor(n/2) */
    idx_t m = (n + 1) >> 1;       /* ceil(n/2)  */
    idx_t r = m >> 1;             /* m/2        */

    seq_t fac_k   = _fact_n(k);
    seq_t fac_m   = _fact_n(m);
    seq_t bin_k_r = _binom(k, r);
    seq_t bin_m_r = _binom(m, r);

    seq_t res = fac_k * fac_m;
    res = res * bin_k_r;
    res = res * bin_m_r;
    return res;
}
