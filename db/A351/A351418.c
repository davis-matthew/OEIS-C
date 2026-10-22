/*
 * A351418  Number of divisors of n that are either of the form p^k (p prime,
 *          k>1) or are nonprime squarefree numbers.
 * https://oeis.org/A351418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n that are either * prime powers
 *            p^k with k>1 or nonprime squarefree numbers * (squarefree
 *            composites). a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void count_helper(size_t idx, size_t cnt, uint64_t *exps,
                         uint64_t *cur_exps, seq_t *acc)
{
    if (idx == cnt) {
        size_t nonzero = 0;
        size_t idx_nonzero = cnt;       /* dummy value */
        bool all_one = true;

        for (size_t i = 0; i < cnt; ++i) {
            if (cur_exps[i] > 0) {
                ++nonzero;
                idx_nonzero = i;
                if (cur_exps[i] != 1) all_one = false;
            }
        }
        if (nonzero == 0) {        /* divisor 1, skip here */
            return;
        }
        if (nonzero == 1) {        /* single prime factor */
            if (cur_exps[idx_nonzero] >= 2) {
                ++(*acc);
            }
        } else {                   /* at least two distinct primes */
            if (all_one) {
                ++(*acc);
            }
        }
        return;
    }

    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        cur_exps[idx] = e;
        count_helper(idx + 1, cnt, exps, cur_exps, acc);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for 0, safety */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* generate all divisors via recursion and count those that satisfy
     * the conditions.  Divisor 1 is always counted. */
    seq_t acc = 0;
    uint64_t cur_exps[64] = {0};
    count_helper(0, cnt, exps, cur_exps, &acc);

    /* include divisor 1 */
    return (seq_t)1 + acc;
}
