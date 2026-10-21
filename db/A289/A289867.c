/*
 * A289867  Primes obtained from other primes by prefixing a 6.
 * https://oeis.org/A289867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 240, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, is_prime_big, next_prime, pow10, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A289867 – primes obtained by prefixing a 6 to a prime.
   The n-th term is the n-th smallest prime of the form 6·10^d + q,
   where q is a d‑digit prime and the result is also prime.
   The function builds a cache incrementally, so repeated calls with
   arbitrary order of n are supported. */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digit_count.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Return the n‑th term of A289867 (1‑based).  For n == 0 the value is
   undefined; the function returns 0 to avoid a crash. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;

    /* Current suffix prime to test */
    static uint64_t next_suffix = 2;     /* first prime to examine */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 16;                  /* initial capacity */
        cache = seq_alloc(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t q = next_suffix;        /* current suffix prime */

        /* Number of decimal digits of q (q is 64‑bit, so digit_count works) */
        unsigned int d = (unsigned int)digit_count(q);

        /* 10^d as a seq_t value */
        seq_t pow10_val = pow10((idx_t)d);

        /* Candidate number: 6·10^d + q */
        seq_t candidate = (seq_t)6 * pow10_val + (seq_t)q;

        /* Keep it if the candidate is prime */
        if (is_prime_big(candidate)) {
            if (cache_len == cache_cap) {          /* grow cache */
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }

        /* Advance to the next suffix prime */
        next_suffix = next_prime(q + 1);
    }

    /* Return the n‑th term (1‑based indexing) */
    return cache[n - 1];
}
