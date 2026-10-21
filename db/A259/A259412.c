/*
 * A259412  Primes of the form 1 - sigma(n) + sigma(n)^2 - sigma(n)^3 +
 *          sigma(n)^4.
 * https://oeis.org/A259412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = primes of the form 1 - σ(n) + σ(n)^2 - σ(n)^3 + σ(n)^4
 *            Returned in the order of increasing n (A259410). Every call
 *            produces the next prime in this list, skipping any n for which
 *            the expression is not prime
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 1017,
 *            u128 n <= 1017, big any n
 * Helpers:   append_candidate, isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

/* Static cache state shared by all calls */
static seq_t *prime_cache = NULL;   /* array of found primes          */
static size_t prime_cache_len = 0;  /* number of primes stored        */
static size_t prime_cache_cap = 0;  /* allocated capacity              */
static idx_t next_tested_n     = 1; /* next n to evaluate             */

/* Return the n‑th prime of the form 1 - σ(n) + σ(n)^2 - σ(n)^3 + σ(n)^4 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* Generate primes until we have at least n of them */
    while (prime_cache_len < (size_t)n) {
        seq_t s   = sigma_of(next_tested_n);        /* σ(n)   */
        seq_t s2  = s * s;                          /* σ^2    */
        seq_t s3  = s2 * s;                         /* σ^3    */
        seq_t s4  = s3 * s;                         /* σ^4    */
        /* Compute 1 - σ + σ^2 - σ^3 + σ^4 */
        seq_t val = s4 - s3;                        /* σ^4 - σ^3   */
        val += s2;                                 /* + σ^2       */
        val -= s;                                  /* - σ         */
        val += 1;                                  /* + 1         */

        if (isPrimeGeneric(val)) {
            /* Append prime to cache */
            append_candidate(&prime_cache, &prime_cache_len,
                             &prime_cache_cap, val);
        }
        ++next_tested_n;
    }

    /* Return the requested element (1‑based index) */
    return prime_cache[n-1];
}
