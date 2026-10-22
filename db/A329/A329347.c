/*
 * A329347  Dirichlet convolution of the identity function with bigomega.
 * https://oeis.org/A329347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * bigomega(n/d)
 * Range:     u8 n <= 95, u16 n <= 8252, u32 n <= 8252, u64 n <= 8252,
 *            u128 n <= 8252, big any n
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/bigomega.h"

/* Recursively enumerate all divisors of N.
   `primes` and `exps` contain the prime factorisation of N
   (`cnt` = number of distinct primes).
   `idx`   : current index in the prime list.
   `cur`   : product of prime powers chosen so far (a divisor of N).
   `N`     : the original number (uint64_t).
   `sum`   : accumulated sum (seq_t). */
static void gen_divs(const uint64_t *primes,
                     const uint64_t *exps,
                     size_t cnt,
                     size_t idx,
                     uint64_t cur,
                     uint64_t N,
                     seq_t *sum)
{
    if (idx == cnt) {
        seq_t term = (seq_t)cur * bigomega(N / cur);
        *sum += term;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t prod = 1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        gen_divs(primes, exps, cnt, idx + 1, cur * prod, N, sum);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N == 0)
        return 0;

    /* Storage for prime factors and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(N, primes, exps, &cnt);

    seq_t sum = 0;
    gen_divs(primes, exps, cnt, 0, 1, N, &sum);
    return sum;
}
