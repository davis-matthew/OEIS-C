/*
 * A304181  If n = Product (p_j^k_j) then a(n) = min{p_j}^min{k_j}.
 * https://oeis.org/A304181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{p_j}^{min{k_j}} where n = Π p_j^k_j
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;                 /* a(1) = 1 by definition */

    /* work with a 64‑bit unsigned integer representation */
    uint64_t m = (uint64_t)n;

    /* arrays large enough for the maximum number of distinct prime factors of a 64‑bit integer */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt = 0;

    /* fully factorize m; primitives are stored in pr[] and ex[] */
    factor_u64(m, pr, ex, &cnt);

    /* find the smallest prime factor and the smallest exponent */
    uint64_t minp    = UINT64_MAX;
    uint64_t minexp  = UINT64_MAX;

    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] < minp)   minp = pr[i];
        if (ex[i] < minexp) minexp = ex[i];
    }

    /* compute minp^minexp in the seq_t domain */
    return seq_pow((seq_t)minp, (idx_t)minexp);
}
