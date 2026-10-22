/*
 * A332877  Arrange the first n primes in a circle in any order. a(n) is the
 *          minimum value of the largest product of two consecutive primes out
 *          of all possible orders.
 * https://oeis.org/A332877
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum possible maximum product of two consecutive *
 *            primes when the first n primes are arranged on a circle. * * The
 *            implementation builds the first n primes once per call and *
 *            evaluates the “interleaving” arrangement which attains the known
 *            * optimum for all n ≥ 2. The primes are cache...
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx */
#include "../utils/build_primes_upto.h"     /* build_primes_upto */

/* Cached prime data */
static uint64_t *prime_cache = NULL;   /* array of at least n primes */
static size_t   prime_count = 0;       /* number of primes stored */

/* Ensure that prime_cache contains at least the first n primes. */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    /* Acquire an upper bound that is guaranteed to contain at least n primes. */
    idx_t limit = _prime_limit_for_idx(n);
    if (limit < n) limit = n;          /* fallback: at least n */

    while (prime_count < (size_t)n) {
        uint64_t *tmp = NULL;
        size_t   tmpc = 0;

        build_primes_upto(limit, &tmp, &tmpc);
        if (tmp == NULL || tmpc == 0) return;   /* unrecoverable */

        /* Replace old cache with the new one. */
        if (prime_cache != NULL) free(prime_cache);
        prime_cache = tmp;
        prime_count = tmpc;

        if (prime_count < (size_t)n) {
            /* Not enough primes yet – increase the limit. */
            limit *= 2;
        }
    }
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n < 2 */

    /* Make sure we have the first n primes in prime_cache. */
    ensure_primes(n);

    seq_t max_prod = 0;

    /* Evaluate the interleaving arrangement:
     * position i has prime index
     *   even i   →  i/2
     *   odd  i   →  n-1-i/2
     */
    for (idx_t i = 0; i < n; ++i) {
        idx_t pi = (i & 1) ? (n - 1 - (i >> 1)) : (i >> 1);
        idx_t j  = (i + 1 < n) ? (i + 1) : 0;
        idx_t pj = (j & 1) ? (n - 1 - (j >> 1)) : (j >> 1);

        seq_t v1 = (seq_t)prime_cache[pi];
        seq_t v2 = (seq_t)prime_cache[pj];
        seq_t prod = v1 * v2;
        if (prod > max_prod) max_prod = prod;
    }

    return max_prod;
}
