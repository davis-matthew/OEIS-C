/*
 * A321663  a(n) = prime(n)^prime(n+2).
 * https://oeis.org/A321663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^prime(n+2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A321663.
   Uses a static cache of primes built by build_primes_upto.
   The cache is rebuilt only when a higher index is requested. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* The sequence is 1‑based. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Static cache of primes. */
    static uint64_t *primes = NULL;
    static size_t  prime_cnt = 0;
    static idx_t   prime_limit_value = 0;

    const idx_t need_idx   = n + 2;                     /* need prime(n) and prime(n+2) */
    const idx_t needed_limit = _prime_limit_for_idx(need_idx);

    /* Rebuild the sieve if the cache is empty or insufficient. */
    if (primes == NULL ||
        (idx_t)prime_cnt < need_idx ||
        prime_limit_value < needed_limit) {

        if (primes != NULL) {
            free(primes);
            primes = NULL;
        }

        build_primes_upto(needed_limit, &primes, &prime_cnt);
        prime_limit_value = needed_limit;
    }

    /* Retrieve the required primes.  The array is zero‑based. */
    const seq_t p1 = (seq_t)primes[n - 1];   /* prime(n)   */
    const seq_t p2 = (seq_t)primes[n + 1];   /* prime(n+2) */

    return big_pow(p1, p2);
}
