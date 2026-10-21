/*
 * A241680  2^p + 11 where p is prime.
 * https://oeis.org/A241680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p_n + 11 where p_n is the nth prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Cache of prime numbers: first n primes stored in prime_cache[0..]
       multiplied by 1 to keep indices 1‑based. */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cache_cnt = 0;

    /* OEIS offset: a(1) is the first term; values for n < 1 are unused. */
    if (n == 0) return (seq_t)0;

    /* Ensure we have at least n primes cached. */
    if (prime_cache == NULL || n > prime_cache_cnt) {
        /* Find an upper bound that guarantees at least n primes. */
        size_t limit = _prime_limit_for_idx(n);
        uint64_t *tmp = NULL;
        size_t tmp_cnt = 0;

        /* Obtain all primes up to the limit. */
        build_primes_upto((idx_t)limit, &tmp, &tmp_cnt);

        /* Replace old cache. */
        free(prime_cache);
        prime_cache = tmp;
        prime_cache_cnt = tmp_cnt;
    }

    /* Retrieve the nth prime. */
    uint64_t p = prime_cache[n - 1];

    /* Compute 2^p + 11.  pow2 returns a seq_t; add 11 casted to seq_t. */
    return pow2((idx_t)p) + (seq_t)11;
}
