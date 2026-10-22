/*
 * A390864  The number of integers k from 1 to n such that gcd(n, k) is a
 *          noninfinitary divisor of n.
 * https://oeis.org/A390864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - Σ_{d∈ID(n)} φ(n/d), where ID(n) = infinitary divisors
 *            of n
 * Range:     u8 n <= 575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_of_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi_of_power.h"

static void dfs(const uint64_t *primes,
                const uint64_t *exps,
                size_t cnt,
                size_t idx,
                uint64_t current_phi,
                uint64_t *sum)
{
    if (idx == cnt) {
        *sum += current_phi;
        return;
    }
    uint64_t e = exps[idx];
    uint64_t sub = e;
    for (;;) {
        /* sub is a submask of e, i.e. an admissible exponent for this prime */
        uint64_t phi_fac = phi_of_power(primes[idx], e - sub);
        dfs(primes, exps, cnt, idx + 1, current_phi * phi_fac, sum);
        if (sub == 0) break;
        sub = (sub - 1) & e;
    }
}

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* factorisation of n: trial division (n <= 2^64-1) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    uint64_t sum = 0;
    dfs(primes, exps, cnt, 0, 1, &sum);

    seq_t result = (seq_t)n - (seq_t)sum;
    return result;
}
