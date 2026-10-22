/*
 * A342956  a(n) = A001222(A001414(n)).
 * https://oeis.org/A342956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A001414(n))
 * Range:     u8 n <= 4662, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses auxiliary functions:
   - factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
   - count_prime_factors(seq_t m)
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;        /* a(1) = 0 as per the sequence */

    /* Factorize n using the provided helper */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt   = 0;

    factor_u64(nn, primes, exps, &cnt);

    /* Compute A001414(n): sum of prime factors of n with multiplicity */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        for (uint64_t j = 0; j < e; ++j) {
            sum += (seq_t)p;
        }
    }

    /* Compute A001222 of that sum: total number of prime factors with multiplicity */
    return count_prime_factors(sum);
}
