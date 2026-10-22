/*
 * A332696  Sum of the proper divisors of n such that d, n/d and n-d are all
 *          squarefree.
 * https://oeis.org/A332696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{d|n, d<n} d * μ(d)^2 * μ(n/d)^2 * μ(n-d)^2
 * Range:     u8 n <= 318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_squarefree.h"

/* Helper: recursively enumerate all divisors of n from its prime factorization. */
static void enumerate_divs(size_t idx, uint64_t cur,
                           uint64_t n,
                           const uint64_t *primes,
                           const uint64_t *exps,
                           size_t cnt,
                           seq_t *sum)
{
    if (idx == cnt) {
        if (cur == n)   /* skip the divisor equal to n (proper divisors only) */
            return;
        if (is_squarefree((seq_t)cur) &&
            is_squarefree((seq_t)(n / cur)) &&
            is_squarefree((seq_t)(n - cur))) {
            *sum += (seq_t)cur;
        }
        return;
    }

    uint64_t power = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        enumerate_divs(idx + 1, cur * power, n, primes, exps, cnt, sum);
        power *= primes[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 */
        return 0;

    /* Factor n into primes and exponents */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    enumerate_divs(0, 1, (uint64_t)n, primes, exps, cnt, &sum);
    return sum;
}
