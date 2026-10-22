/*
 * A345276  a(n) = Sum_{d|n} (n-d)^abs(mu(n/d)).
 * https://oeis.org/A345276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n-d)^abs(mu(n/d))
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   factor_u64, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mobius.h"

/* Recursive enumeration of all divisors of n.
   For each divisor d, compute e = n/d, mu(e),
   and add either 1 (if mu(e)==0) or n-d (otherwise). */
static void rec_divs(idx_t n, size_t pos, idx_t cur_d, seq_t nn, seq_t *sum,
                     size_t cnt, const uint64_t *pr, const uint64_t *exps)
{
    if (pos == cnt) {
        idx_t e = n / cur_d;
        seq_t mu = mobius(e);
        if (mu == 0)
            *sum += 1;
        else
            *sum += nn - cur_d;
        return;
    }

    /* exponent 0 for current prime */
    rec_divs(n, pos + 1, cur_d, nn, sum, cnt, pr, exps);

    /* exponents 1 .. exps[pos] */
    idx_t d = cur_d;
    for (uint64_t e = 1; e <= exps[pos]; ++e) {
        d *= (idx_t)pr[pos];
        rec_divs(n, pos + 1, d, nn, sum, cnt, pr, exps);
    }
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sum = 0;
    uint64_t pr[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, exps, &cnt);

    /* Enumerate all divisors of n and accumulate the sum. */
    rec_divs(n, 0, 1, nn, &sum, cnt, pr, exps);
    return sum;
}
