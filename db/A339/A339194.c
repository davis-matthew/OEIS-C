/*
 * A339194  Sum of all squarefree semiprimes with greater prime factor
 *          prime(n).
 * https://oeis.org/A339194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * Sum_{k=1}^{n-1} prime(k) (identity from OEIS)
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   _prime_limit_for_idx, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/sieve_primes.h"

seq_t a(idx_t n)
{
    /* Handle the special case for n = 1 */
    if (n <= 1) return (seq_t)0;

    /* Static caches for primes and prefix sums */
    static uint64_t *primes = NULL;      /* array of primes up to the current limit */
    static seq_t *cumsum = NULL;        /* prefix sums of primes: cumsum[i] = sum of first i primes (i>=0) */
    static size_t prime_count = 0;       /* number of primes currently stored (== size of primes array) */
    static size_t max_n_processed = 0;   /* maximum index n for which primes were built */

    /* If we haven't processed this n yet, or we need more primes, rebuild the cache */
    if (n > max_n_processed)
    {
        /* Compute a suitable prime limit for the nth prime */
        idx_t limit = _prime_limit_for_idx(n);

        /* Allocate space for new primes */
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;

        sieve_primes(limit, &new_primes, &new_cnt);

        /* Free old data */
        free(primes);
        free(cumsum);

        /* Store the new data */
        primes = new_primes;
        prime_count = new_cnt;
        max_n_processed = n;

        /* Allocate and compute the cumulative sum array */
        cumsum = (seq_t *)malloc((prime_count + 1) * sizeof(seq_t));
        cumsum[0] = 0;
        for (size_t i = 0; i < prime_count; ++i)
        {
            cumsum[i + 1] = cumsum[i] + (seq_t)primes[i];
        }
    }

    /* Compute the result: prime(n) * sum_{k=1}^{n-1} prime(k) */
    seq_t prime_n   = (seq_t)primes[n - 1];
    seq_t sum_prev  = cumsum[n - 1];
    return prime_n * sum_prev;
}
