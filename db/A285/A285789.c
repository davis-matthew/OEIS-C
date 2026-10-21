/*
 * A285789  Primes equal to a pentagonal number plus 1.
 * https://oeis.org/A285789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form k*(3*k-1)/2 + 1 (pentagonal numbers + 1)
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Cache for already computed terms so that calls in any order are fast. */
static seq_t *cache      = NULL;   /* stored sequence values */
static size_t cache_len  = 0;      /* number of values in the cache */
static size_t cache_cap  = 0;      /* allocated capacity for the cache   */
static idx_t  last_k     = 0;      /* largest k that has been processed */

/* Return the nth term of A285789 (1‑based). */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Continue searching from the last processed k until we have enough terms. */
    for (idx_t k = last_k + 1; cache_len < n; ++k)
    {
        last_k = k;
        seq_t kk = (seq_t)k;                    /* k in seq_t form */
        seq_t term = kk * 3 - 1;                /* 3k - 1  */
        seq_t candidate = kk * term / 2 + 1;    /* k*(3k-1)/2 + 1 */

        if (is_prime_big(candidate))
            append_candidate(&cache, &cache_len, &cache_cap, candidate);
    }

    return cache[n - 1];
}
