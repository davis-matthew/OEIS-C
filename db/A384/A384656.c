/*
 * A384656  a(n) = Sum_{k=1..n} A051903(ugcd(n,k)), where ugcd(n,k) is the
 *          greatest divisor of k that is a unitary divisor of n.
 * https://oeis.org/A384656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A051903(ugcd(n,k))
 * Range:     u8 n <= 347, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t compute_A051903(idx_t m) {
    if (m <= 1) return 0;
    uint64_t pr[16];
    uint64_t ex[16];
    size_t cnt = 0;
    factor_u64((uint64_t)m, pr, ex, &cnt);
    seq_t mx = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > mx) mx = (seq_t)ex[i];
    }
    return mx;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* factor n to get its prime powers */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t np = 0;
    factor_u64((uint64_t)n, primes, exps, &np);

    /* generate all unitary divisors of n */
    idx_t unitary[256];
    seq_t a_vals[256];
    size_t unitary_cnt = 1;
    unitary[0] = 1;
    a_vals[0] = 0;              /* A051903(1) = 0 */

    for (size_t i = 0; i < np; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t powp = 1;
        for (uint64_t j = 1; j <= e; ++j) powp *= p;

        size_t cur = unitary_cnt;
        for (size_t j = 0; j < cur; ++j) {
            unitary[unitary_cnt] = unitary[j] * powp;
            a_vals[unitary_cnt]  = compute_A051903(unitary[unitary_cnt]);
            ++unitary_cnt;
        }
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t best = 1;
        seq_t best_val = 0;   /* value for divisor 1 */
        for (size_t i = 0; i < unitary_cnt; ++i) {
            idx_t d = unitary[i];
            if (k % d == 0 && d > best) {
                best = d;
                best_val = a_vals[i];
            }
        }
        sum += best_val;
    }
    return sum;
}
