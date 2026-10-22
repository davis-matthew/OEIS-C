/*
 * A327626  Expansion of Sum_{k>=1} x^(k^3) / (1 - x^(k^3))^2.
 * https://oeis.org/A327626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e||n} (p^{e+3} - p^{e mod 3}) / (p^3 - 1) ,
 *            multiplicative
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1298, u64 n <= 9593,
 *            u128 n <= 9593, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        idx_t e = ex[i];
        seq_t p_e3 = seq_pow(p, e + 3);
        seq_t p_emod3 = seq_pow(p, e % 3);
        seq_t numerator = p_e3 - p_emod3;
        seq_t p3 = seq_pow(p, 3);
        seq_t denom = p3 - 1;
        seq_t term = numerator / denom;
        result = result * term;
    }
    return result;
}
