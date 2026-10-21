/*
 * A290116  Primes of the form k! / 2 - 1.
 * https://oeis.org/A290116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime of the form k!/2 – 1 * (k starts at 3,
 *            the first prime comes from 3! ). * The function generates
 *            factorials incrementally and * stores the primes it finds in a
 *            static cache
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   grow_seq_array, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;      /* cache of primes found */
static size_t cache_len = 0;     /* number of cached primes */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* current factorial value; fact = last_k! */
static seq_t fact = 1;
static idx_t last_k = 1;         /* factorial of last_k (initially 1! = 1) */

seq_t a(idx_t n)
{
    /* return cached result if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow cache if necessary (using helper to handle arbitrary‑precision types) */
    if (cache_len >= cache_cap) {
        /* grow by doubling capacity or start at 16 */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* compute terms until we have at least n primes */
    while (cache_len < n) {
        /* advance to next k and update factorial */
        ++last_k;
        fact = mul_small(fact, (unsigned int)last_k);   /* fact = k! */

        /* candidate = (fact - 2) / 2 */
        seq_t cand = fact;
        cand = cand - (seq_t)2;
        cand = cand / (seq_t)2;

        /* check primality */
        if (is_prime_big(cand)) {
            /* ensure capacity for one more element */
            if (cache_len >= cache_cap) {
                grow_seq_array(&cache, &cache_cap, cache_cap * 2);
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
