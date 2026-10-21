/*
 * A241678  2^p - 5 where p is prime.
 * https://oeis.org/A241678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^p_n - 5, where p_n is the n-th prime
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */

    /* Static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t limit_used = 0;

    /* Determine the numeric limit needed to generate at least n primes. */
    idx_t needed_limit = _prime_limit_for_idx(n);

    /* Rebuild the prime table if it is too small or uninitialized. */
    if (primes == NULL || needed_limit > limit_used) {
        free(primes);
        build_primes_upto(needed_limit, &primes, &prime_count);
        limit_used = needed_limit;
    }

    /* Ensure that we actually have n primes; if not, increase the limit. */
    while ((size_t)(n - 1) >= prime_count) {
        idx_t new_limit = limit_used * 2 + 1;
        free(primes);
        build_primes_upto(new_limit, &primes, &prime_count);
        limit_used = new_limit;
    }

    /* Fetch the n-th prime (0‑indexed array). */
    uint64_t p = primes[n - 1];

    /* Compute 2^p - 5.  The helper pow2 handles large powers in arbitrary
       precision when seq_t is large.  Cast is safe because 5 is small. */
    seq_t val = pow2((idx_t)p);
    val -= (seq_t)5;
    return val;
}
