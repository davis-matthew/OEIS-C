/*
 * A364092  Sum of divisors of 5*n-1 of form 5*k+1.
 * https://oeis.org/A364092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of divisors of (5*n-1) that are ≡ 1 (mod 5)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void dfs_sum(int idx, seq_t cur, const uint64_t *pr, const uint64_t *exp,
                    int count, seq_t *sumout)
{
    if (idx == count) {
        if (cur % 5 == 1) {
            *sumout += cur;
        }
        return;
    }
    seq_t p = (seq_t)pr[idx];
    seq_t cur_tmp = cur;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        dfs_sum(idx + 1, cur_tmp, pr, exp, count, sumout);
        cur_tmp *= p;
    }
}

static seq_t sum_divisors_5kplus1(uint64_t m)
{
    /* Factor m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt;
    factor_u64(m, primes, exps, &prime_cnt);

    seq_t sum = 0;
    dfs_sum(0, (seq_t)1, primes, exps, (int)prime_cnt, &sum);
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)5 * (uint64_t)n - 1;
    return sum_divisors_5kplus1(m);
}
