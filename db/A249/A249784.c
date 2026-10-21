/*
 * A249784  Number of divisors of n^(n^n).
 * https://oeis.org/A249784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of divisors of n^(n^n) * a(n) = Π_{p^e || n} (e *
 *            n^n + 1) * where the product runs over all distinct prime
 *            divisors p of n * with multiplicity e
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case first. */
    if (n == 1)
    {
        /* 1^(1^1) = 1, divisors{1} = 1 */
        return (seq_t)1;
    }

    /* Factor n: arrays to hold primes and exponents. */
    /* For n <= 10000, 64 slots is more than enough. */
    const size_t MAX_FACTS = 64;
    uint64_t primes[MAX_FACTS];
    uint64_t exps[MAX_FACTS];
    size_t num_factors = 0;

    factor_u64(n, primes, exps, &num_factors);

    /* Compute n^n as a big integer. */
    seq_t pow_n = big_pow((seq_t)n, (seq_t)n);

    /* Result initialized to 1 (multiplicative identity). */
    seq_t result = (seq_t)1;

    /* For each prime factor, multiply (e*n^n + 1) into the result. */
    for (size_t i = 0; i < num_factors; ++i)
    {
        /* term = e * n^n + 1 */
        seq_t term = pow_n * (seq_t)exps[i] + (seq_t)1;
        result *= term;
    }

    return result;
}
