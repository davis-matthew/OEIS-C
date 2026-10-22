/*
 * A305748  Distance of a prime number from the average of the next two
 *          consecutive prime numbers.
 * https://oeis.org/A305748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_{n+1} + p_{n+2}) / 2 – p_n where p_k is the k‑th
 *            prime. The function is memoised; it keeps a growing cache of
 *            primes and enlarges it only when the requested index is beyond
 *            the current cache. All arithmetic that may overflow a bounded
 *            type (seq_t) is performed in a wider unsigned 64‑bit...
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache      = NULL;   /* array of primes */
static size_t   prime_cache_count = 0;     /* number of primes stored */

/* Return a(n) for n ≥ 1.  For n < 1 the behaviour is undefined; we
   return 0 for safety. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Need primes up to index n+2 (1‑based).  The array is 0‑based. */
    idx_t required = n + 2;
    if (prime_cache_count < (size_t)required) {
        /* Compute an upper bound for the (required)‑th prime. */
        idx_t limit = _prime_limit_for_idx(required);

        /* Rebuild the prime table with the larger limit.  The old array
           is freed automatically by build_primes_upto when a new pointer
           is supplied, but we free it explicitly to be safe. */
        free(prime_cache);

        size_t cnt = 0;
        build_primes_upto(limit, &prime_cache, &cnt);
        prime_cache_count = cnt;
    }

    /* Retrieve the required primes.  Use uint64_t for the intermediate
       arithmetic to avoid overflow of seq_t. */
    uint64_t p_prev   = prime_cache[n - 1];     /* p_n   */
    uint64_t p_next1  = prime_cache[n];        /* p_{n+1} */
    uint64_t p_next2  = prime_cache[n + 1];    /* p_{n+2} */

    uint64_t sum = p_next1 + p_next2;          /* p_{n+1}+p_{n+2} */
    seq_t avg = (seq_t)(sum / 2);              /* (p_{n+1}+p_{n+2})/2 */

    seq_t prev = (seq_t)p_prev;                /* p_n as seq_t */
    return avg - prev;
}
