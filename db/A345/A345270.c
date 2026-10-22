/*
 * A345270  a(n) = Sum_{d|n} d^tau(n/d).
 * https://oeis.org/A345270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^tau(n/d)
 * Range:     u8 n <= 19, u16 n <= 71, u32 n <= 359, u64 n <= 1919,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Recursive helper: enumerate all divisors of n.
 * pos   : current prime index being processed.
 * cur   : product of selected prime powers so far (the divisor d).
 * tau   : product of (exponent_of_n / selected_exponent + 1) over processed primes,
 *         which equals τ(n/d) for the current divisor.
 * sum   : pointer to the accumulation of the sum a(n). */
static void dfs(size_t pos, seq_t cur, seq_t tau,
                const uint64_t *pr, const uint64_t *ex,
                size_t cnt, seq_t *sum)
{
    if (pos == cnt) {
        *sum += big_pow(cur, tau);
        return;
    }

    seq_t cur_value = cur;
    /* iterate over possible exponents for this prime */
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        seq_t tau_factor = (seq_t)(ex[pos] - e + 1);
        dfs(pos + 1,
            cur_value,
            tau * tau_factor,
            pr,
            ex,
            cnt,
            sum);
        if (e < ex[pos])
            cur_value *= (seq_t)pr[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n == 0, return 0 for safety */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n: primes[] contains prime bases, exps[] their exponents */
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    dfs(0, (seq_t)1, (seq_t)1,
        primes, exps, cnt, &sum);
    return sum;
}
