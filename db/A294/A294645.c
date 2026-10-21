/*
 * A294645  a(n) = Sum_{d|n} d^(n+1).
 * https://oeis.org/A294645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n+1) (OEIS A294645)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

static void gen_divs(size_t idx, seq_t cur, const uint64_t *pr, const uint64_t *ex,
                     size_t cnt, seq_t *sum, idx_t power)
{
    if (idx == cnt) {
        *sum += seq_pow(cur, power);
        return;
    }
    const uint64_t p = pr[idx];
    seq_t cur_mult = cur;
    for (unsigned e = 0; e <= ex[idx]; ++e) {
        gen_divs(idx + 1, cur_mult, pr, ex, cnt, sum, power);
        cur_mult = cur_mult * (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* safety: OEIS offset starts at 1 */
        return 0;

    /* factorize n; 64 primes are more than enough for n <= 10^4 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    idx_t power = n + 1;
    gen_divs(0, (seq_t)1, primes, exps, cnt, &sum, power);
    return sum;
}
