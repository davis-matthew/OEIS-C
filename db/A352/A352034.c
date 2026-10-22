/*
 * A352034  Sum of the 6th powers of the odd proper divisors of n.
 * https://oeis.org/A352034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^6
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 81, u64 n <= 3253,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void dfs(const uint64_t *odd_pr, const uint64_t *odd_ex, size_t odd_cnt,
                size_t idx, seq_t cur, seq_t n, seq_t *sum)
{
    if (idx == odd_cnt) {
        if (cur < n) {
            seq_t x2 = cur * cur;
            seq_t x4 = x2 * x2;
            seq_t x6 = x4 * x2;
            *sum += x6;
        }
        return;
    }
    seq_t pf = 1;
    seq_t p = (seq_t)odd_pr[idx];
    for (size_t k = 0; k <= odd_ex[idx]; ++k) {
        seq_t new_cur = cur * pf;
        dfs(odd_pr, odd_ex, odd_cnt, idx + 1, new_cur, n, sum);
        pf *= p;
    }
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n <= 1)
        return sum;

    uint64_t tmp_n = (uint64_t)n;
    uint64_t pr[16], ex[16];
    size_t cnt = 0;
    factor_u64(tmp_n, pr, ex, &cnt);

    /* Collect odd primes only */
    uint64_t odd_pr[16], odd_ex[16];
    size_t odd_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] & 1) {
            odd_pr[odd_cnt] = pr[i];
            odd_ex[odd_cnt] = ex[i];
            odd_cnt++;
        }
    }

    seq_t seq_n = (seq_t)tmp_n;

    dfs(odd_pr, odd_ex, odd_cnt, 0, 1, seq_n, &sum);
    return sum;
}
