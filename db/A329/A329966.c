/*
 * A329966  a(n) = n! * Sum_{d|n} binomial(n-1,d-1) / d!.
 * https://oeis.org/A329966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} binomial(n-1,d-1) / d!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"

/* Recursive enumeration of all positive divisors of n.
   *sum accumulates the terms of the required sum. */
static void enumerate_divs(idx_t idx, idx_t cur,
                           uint64_t *primes, uint64_t *exps,
                           size_t num_primes, idx_t n,
                           seq_t fact_n, seq_t *sum)
{
    if (idx == num_primes) {
        /* cur is a divisor d of n */
        seq_t fact_d = factorial(cur);
        seq_t bin    = binomial(n - 1, cur - 1);
        seq_t term   = (fact_n / fact_d) * bin;
        *sum += term;
        return;
    }

    uint64_t p   = primes[idx];
    idx_t dcur    = cur;
    /* iterate over all exponents of this prime */
    for (unsigned int e = 0; e <= exps[idx]; ++e) {
        enumerate_divs(idx + 1, dcur, primes, exps,
                       num_primes, n, fact_n, sum);
        dcur *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1; return 0 for safety */
        return (seq_t)0;

    seq_t fact_n = factorial(n);

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   num_primes;
    factor_u64(n, primes, exps, &num_primes);

    seq_t sum = 0;
    enumerate_divs(0, (idx_t)1, primes, exps,
                   num_primes, n, fact_n, &sum);
    return sum;
}
