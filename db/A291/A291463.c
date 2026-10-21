/*
 * A291463  a(n) is the product of n-th prime number with the distance to the
 *          next prime.
 * https://oeis.org/A291463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * (prime(n+1) - prime(n))
 * Range:     u8 n <= 14, u16 n <= 428, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* get an upper bound that guarantees at least n+1 primes */
    idx_t limit_idx = _prime_limit_for_idx(n + 1);
    /* build the list of primes up to that limit */
    uint64_t *primes = NULL;
    size_t  count   = 0;
    build_primes_upto(limit_idx, &primes, &count);

    /* if we unexpectedly got fewer primes, grow the limit and rebuild */
    while (count < (size_t)(n + 1))
    {
        free(primes);  /* free previous array */
        limit_idx = _prime_limit_for_idx(count + 1000); /* increase bound */
        build_primes_upto(limit_idx, &primes, &count);
    }

    /* fetch the required primes (n is 1‑based) */
    seq_t p_n     = (seq_t)primes[n - 1];
    seq_t p_next  = (seq_t)primes[n];
    seq_t gap     = p_next - p_n;

    seq_t result = p_n * gap;

    free(primes);
    return result;
}
