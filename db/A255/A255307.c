/*
 * A255307  Concatenation of the first n entries of the difference sequence of
 *          prime numbers (see A001223).
 * https://oeis.org/A255307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the first n prime gaps. Gap i = p_{i+1}
 *            - p_i where p_i is the i‑th prime. The algorithm generates the
 *            first n+1 primes using a sieve up to an upper bound for the
 *            (n+1)th prime, then builds the concatenated number by
 *            multiplying the current result by appropriate power of t...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx */
#include "../utils/build_primes_upto.h"     /* build_primes_upto */
#include "../utils/pow10.h"                /* pow10 */

static inline idx_t decimal_digits(uint64_t v)
{
    idx_t d = 0;
    do {
        d++;
        v /= 10;
    } while (v);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    /* Upper bound for (n+1)-th prime */
    idx_t limit = _prime_limit_for_idx(n + 1);
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;

    /* Generate all primes ≤ limit */
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Ensure we have at least n+1 primes.  The bound is safe, but
       check anyway; if not, increase the limit (rare). */
    if (prime_cnt <= (size_t)n) {
        free(primes);
        /* Retry with a larger bound if necessary */
        limit = _prime_limit_for_idx(n + 2);
        build_primes_upto(limit, &primes, &prime_cnt);
    }

    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        uint64_t diff = primes[i + 1] - primes[i];
        idx_t digits = decimal_digits(diff);
        seq_t mult = pow10(digits);   /* 10^digits as seq_t */
        result = result * mult + (seq_t)diff;
    }

    free(primes);
    return result;
}
