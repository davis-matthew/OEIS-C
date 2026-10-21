/*
 * A275786  a(n) = Product_{d|n} T(d) where T(x) = x*(x+1)/2 = A000217(x) =
 *          x-th triangular number.
 * https://oeis.org/A275786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{d|n} d*(d+1)/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 71, u128 n <= 239,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void _enum_divs(idx_t cur, size_t idx, uint64_t *pr, int *ex, size_t cnt, seq_t *prod)
{
    if (idx == cnt) {
        seq_t tri = ((seq_t)cur) * (((seq_t)cur) + 1) / 2;
        *prod = *prod * tri;
        return;
    }
    uint64_t p = pr[idx];
    int e = ex[idx];
    uint64_t v = cur;
    for (int k = 0; k <= e; ++k) {
        _enum_divs(v, idx + 1, pr, ex, cnt, prod);
        v *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out‑of‑range guard, not used */
    uint64_t primes[64];
    uint64_t exps_raw[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps_raw, &cnt);

    int exps[64];
    for (size_t i = 0; i < cnt; ++i) exps[i] = (int)exps_raw[i];

    seq_t result = 1;
    _enum_divs(1, 0, primes, exps, cnt, &result);
    return result;
}
