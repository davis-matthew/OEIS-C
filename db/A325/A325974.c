/*
 * A325974  Arithmetic mean of {sum of non-unitary divisors} and {sum of
 *          nonsquarefree divisors}: a(n) = (1/2)*(A048146(n) + A162296(n)).
 * https://oeis.org/A325974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A048146(n)+A162296(n))/2
 * Range:     u8 n <= 143, u16 n <= 9050, u32 n <= 9050, u64 n <= 9050,
 *            u128 n <= 9050, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static void dfs(size_t pos, seq_t cur_val, int unitary_flag, int squarefree_flag,
                const uint64_t *pr, const uint64_t *ex, size_t cnt,
                seq_t *sum_nonunitary, seq_t *sum_nonsquarefree)
{
    if (pos == cnt) {
        if (!unitary_flag) *sum_nonunitary += cur_val;
        if (!squarefree_flag) *sum_nonsquarefree += cur_val;
        return;
    }
    uint64_t p  = pr[pos];
    uint64_t eN = ex[pos];
    seq_t val = cur_val;
    for (uint64_t eD = 0; eD <= eN; ++eD) {
        int new_unitary_flag   = unitary_flag && (eD == 0 || eD == eN);
        int new_squarefree_flag = squarefree_flag && (eD <= 1);
        dfs(pos + 1, val, new_unitary_flag, new_squarefree_flag,
            pr, ex, cnt, sum_nonunitary, sum_nonsquarefree);
        if (eD < eN) val = val * (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    seq_t sum_nonunitary   = 0;
    seq_t sum_nonsquarefree = 0;
    dfs(0, (seq_t)1, 1, 1, primes, exps, prime_cnt,
        &sum_nonunitary, &sum_nonsquarefree);
    return (sum_nonunitary + sum_nonsquarefree) / 2;
}
