/*
 * A388013  Number of divisors d of n for which sigma(d)/d >= 5/3.
 * https://oeis.org/A388013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | d divides n and sigma(d)/d ≥ 5/3 }
 * Range:     u8 n <= 9720, u16 n <= 9720, u32 n <= 9720, u64 n <= 9720,
 *            u128 n <= 9720, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void count_divs(size_t pos, uint64_t cur_d, uint64_t cur_sigma,
                       uint64_t *pr, uint64_t *ex, size_t cnt, uint64_t *res)
{
    if (pos == cnt) {
        /* Check σ(d)/d ≥ 5/3  ⇔  σ(d)·3 ≥ d·5 */
        if (cur_sigma * 3 >= cur_d * 5)
            (*res)++;
        return;
    }

    uint64_t p = pr[pos];
    uint64_t maxe = ex[pos];
    uint64_t powp = 1;          /* p^e */

    for (uint64_t e = 0; e <= maxe; ++e) {
        uint64_t new_d = cur_d * powp;          /* d part */

        /* σ(p^e) = (p^(e+1) – 1) / (p – 1) */
        uint64_t powp_next = powp * p;           /* p^(e+1) */
        uint64_t numer = powp_next - 1;
        uint64_t denom = p - 1;
        uint64_t sigma_part = numer / denom;

        uint64_t new_sigma = cur_sigma * sigma_part;
        count_divs(pos + 1, new_d, new_sigma, pr, ex, cnt, res);

        powp = powp_next;                      /* advance to next exponent */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* sequence is only defined for n ≥ 1 (offset 1) */

    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];

    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t result = 0;
    count_divs(0, 1, 1, primes, exps, cnt, &result);
    return (seq_t)result;
}
