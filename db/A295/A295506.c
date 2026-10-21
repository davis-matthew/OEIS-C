/*
 * A295506  a(n) = Sum_{d|n} mu(n/d)*5^(d-1).
 * https://oeis.org/A295506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * 5^(d-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

static void dfs(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                size_t idx, uint64_t cur_d, int mu_zero, int mu_parity,
                seq_t *sum)
{
    if (idx == cnt) {
        if (mu_zero) return;
        seq_t pow = big_pow((seq_t)5, (seq_t)cur_d - 1);
        if (mu_parity)
            *sum -= pow;
        else
            *sum += pow;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    uint64_t d_local = cur_d;
    for (uint64_t f = 0; f <= e; ++f) {
        int mz = mu_zero;
        int mp = mu_parity;
        uint64_t kexp = e - f;           /* exponent of k = n/d */
        if (kexp > 1)
            mz = 1;
        else if (kexp == 1)
            mp ^= 1;
        dfs(pr, ex, cnt, idx + 1, d_local, mz, mp, sum);
        if (f < e)
            d_local *= p;                /* next divisor */
    }
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    uint64_t pr_arr[64];
    uint64_t ex_arr[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr_arr, ex_arr, &cnt);
    dfs(pr_arr, ex_arr, cnt, 0, 1ULL, 0, 0, &total);
    return total;
}
