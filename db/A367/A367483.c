/*
 * A367483  Sum of the arithmetic derivatives of the nontrivial divisors of n
 *          whose arithmetic derivative is coprime to n.
 * https://oeis.org/A367483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   by definition * a(n) = Sum_{d|n, d>1, gcd(n, d')=1} d', * where
 *            d' = d * SUM_{p|d} (exp_p(d)/p)
 * Range:     u8 n <= 209, u16 n <= 9355, u32 n <= 9355, u64 n <= 9355,
 *            u128 n <= 9355, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */

static void gen_divs(int pos, seq_t cur_d, int *exp_d,
                     const uint64_t *pr, const int *exps, int pcnt,
                     idx_t n, seq_t *sum)
{
    if (pos == pcnt) {
        if (cur_d > (seq_t)1) {
            seq_t dprime = (seq_t)0;
            for (int i = 0; i < pcnt; ++i) {
                if (exp_d[i] > 0) {
                    seq_t term = cur_d;
                    term /= (seq_t)pr[i];           /* d / p_i */
                    term *= (seq_t)exp_d[i];        /* * e_i */
                    dprime += term;
                }
            }
            if (gcd(dprime, (seq_t)n) == (seq_t)1) {
                *sum += dprime;
            }
        }
        return;
    }
    seq_t cur = cur_d;
    for (int e = 0; e <= exps[pos]; ++e) {
        exp_d[pos] = e;
        gen_divs(pos + 1, cur, exp_d, pr, exps, pcnt, n, sum);
        if (e < exps[pos]) {
            cur *= (seq_t)pr[pos];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)0;
    }

    /* factors of n */
    uint64_t pr[64];
    uint64_t expp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, expp, &cnt);

    int exps[64];
    for (size_t i = 0; i < cnt; ++i) {
        exps[i] = (int)expp[i];
    }

    int exp_d[64] = {0};
    seq_t sum = (seq_t)0;

    gen_divs(0, (seq_t)1, exp_d, pr, exps, (int)cnt, n, &sum);
    return sum;
}
