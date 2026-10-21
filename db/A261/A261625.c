/*
 * A261625  Number of primes p <= n such that (p-1)*n+1 is prime.
 * https://oeis.org/A261625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{p ≤ n | p prime and (p-1)*n+1 is prime}
 * Range:     u8 n <= 16, u16 n <= 513, u32 n <= 513, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n < 2)
        return 0;

    /* Generate all primes ≤ n */
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(n, &primes, &prime_cnt);
    if (primes == NULL)
        return 0;   /* should not happen */

    /* Count primes satisfying the condition */
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        seq_t cand = (seq_t)(p - 1) * (seq_t)n + (seq_t)1;
        if (is_prime_big(cand))
            ++count;
    }

    free(primes);
    return count;
}
