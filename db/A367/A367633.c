/*
 * A367633  Sum of the nontrivial divisors of n whose arithmetic derivative is
 *          coprime to n.
 * https://oeis.org/A367633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d>1, gcd(n,d')=1} d, where d' is the arithmetic
 *            derivative of d
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static void dfs(int idx, uint64_t cur, const uint64_t *pr, const int *exp, int nf,
                seq_t *sum, seq_t n_seq, int *cur_exp)
{
    if (idx == nf) {
        if (cur > 1u) {
            seq_t d_seq = (seq_t)cur;
            seq_t d_der = 0;
            for (int i = 0; i < nf; ++i) {
                if (cur_exp[i]) {
                    seq_t term = d_seq / (seq_t)pr[i];
                    term *= cur_exp[i];
                    d_der += term;
                }
            }
            if (gcd(n_seq, d_der) == (seq_t)1) {
                *sum += d_seq;
            }
        }
        return;
    }
    for (int e = 0; e <= exp[idx]; ++e) {
        cur_exp[idx] = e;
        uint64_t new_cur = cur;
        for (int k = 0; k < e; ++k) new_cur *= pr[idx];
        dfs(idx + 1, new_cur, pr, exp, nf, sum, n_seq, cur_exp);
    }
}

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)0;

    seq_t total = 0;
    uint64_t primes[16];
    uint64_t exp64[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exp64, &cnt);

    int exp[16];
    int cur_exp[16];
    for (size_t i = 0; i < cnt; ++i) {
        exp[i] = (int)exp64[i];
        cur_exp[i] = 0;
    }

    dfs(0, 1u, primes, exp, (int)cnt, &total, (seq_t)n, cur_exp);
    return total;
}
