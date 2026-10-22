/*
 * A385130  The sum of divisors of n whose maximum exponent in their prime
 *          factorization is even.
 * https://oeis.org/A385130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * (1 - (max exponent of d mod 2))
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Index offset: 1  (a(1) = 1) */

/* The tool provides a helper to obtain a prime factorisation of a 64‑bit
   integer.  The function is declared in "../utils/factor_u64.h". */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursively enumerate all divisors from the prime factorisation. */
/* `primes[i]` ^ `exps[i]` are the ith prime factor and its exponent.
   `cur` is the divisor constructed so far.
   `cur_max` is the maximum exponent seen in `cur`. */
static void dfs_rec(const uint64_t *primes, const uint64_t *exps,
                    size_t cnt, size_t idx, seq_t cur, int cur_max,
                    seq_t *sum)
{
    if (idx == cnt) {
        /* Include `cur` if its maximum exponent is even. */
        if ((cur_max & 1) == 0) {
            *sum += cur;
        }
        return;
    }

    seq_t mult = 1;                 /* primes[idx]^k */
    for (uint64_t k = 0; k <= exps[idx]; ++k) {
        /* Update the maximum exponent seen so far. */
        int next_max = (k > (uint64_t)cur_max) ? (int)k : cur_max;
        dfs_rec(primes, exps, cnt, idx + 1, cur * mult, next_max, sum);
        /* Prepare for next exponent. */
        mult *= (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* Handle n = 1 directly; the recursive routine also works for it. */
    if (n == 0) return 0;          /* out of range – not expected by the
                                      OEIS offset but safe. */

    /* Factorise n. */
    uint64_t primes[64];           /* buffer for prime factors   */
    uint64_t exps[64];             /* corresponding exponents   */
    size_t  cnt = 0;               /* number of distinct primes */

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs_rec(primes, exps, cnt, 0, (seq_t)1, 0, &sum);
    return sum;
}
