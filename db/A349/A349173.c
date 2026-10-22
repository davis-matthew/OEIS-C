/*
 * A349173  Dirichlet convolution of A003415 with A003959, where A003415 is
 *          the arithmetic derivative and A003959 is fully multiplicative with
 *          a(p) = (p+1).
 * https://oeis.org/A349173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(d) * A003959(n/d) * A003415 is the
 *            arithmetic derivative, A003959 is fully * multiplicative with
 *            a(p) = p+1
 * Range:     u8 n <= 23, u16 n <= 575, u32 n <= 8934, u64 n <= 8934,
 *            u128 n <= 8934, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

static void dfs(uint64_t idx,
                seq_t curr,
                uint64_t *pr,
                uint64_t *ex,
                size_t cnt,
                uint64_t *curr_e,
                seq_t *res)
{
    if (idx == cnt) {
        /* compute derivative of curr */
        seq_t deriv = 0;
        for (size_t j = 0; j < cnt; ++j) {
            if (curr_e[j] > 0) {
                deriv += (curr / (seq_t)pr[j]) * (seq_t)curr_e[j];
            }
        }

        /* compute A003959(n/curr) = product (p+1)^(ex[i]-curr_e[i]) */
        seq_t b_val = 1;
        for (size_t j = 0; j < cnt; ++j) {
            uint64_t diff = ex[j] - curr_e[j];
            if (diff) {
                b_val *= big_pow((seq_t)(pr[j] + 1), (seq_t)diff);
            }
        }

        *res += deriv * b_val;
        return;
    }

    /* iterate over possible exponent of current prime */
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        curr_e[idx] = e;
        seq_t new_curr = curr;
        for (uint64_t k = 0; k < e; ++k) {
            new_curr *= (seq_t)pr[idx];
        }
        dfs(idx + 1, new_curr, pr, ex, cnt, curr_e, res);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t curr_e[64] = {0};
    seq_t result = 0;
    dfs(0, (seq_t)1, primes, exps, cnt, curr_e, &result);
    return result;
}
