/*
 * A244736  Numbers k such that (prime(k) mod 5) is even.
 * https://oeis.org/A244736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A244736: a(n) is the n-th positive integer k such that prime(k) mod 5 is even
   (i.e. the residue 0, 2 or 4).  The function builds the sequence on demand
   and caches the already found terms so that calls in arbitrary order are
   fast. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* n is always ≥ 1 by the definition of the sequence. */
    static seq_t *cache = NULL;           /* array of found indices k     */
    static size_t cache_len = 0;          /* number of stored elements    */
    static size_t cache_cap = 0;          /* capacity of cache            */
    static idx_t last_k = 0;              /* last tested prime index      */

    /* Extend the cache until it contains at least n elements. */
    if (cache_len < (size_t)n) {
        size_t needed = (size_t)n;
        if (needed > cache_cap) {
            /* Grow the cache geometrically. */
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            while (new_cap < needed) new_cap *= 2;
            ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
        }

        /* Generate further terms until we have the required number. */
        while (cache_len < (size_t)n) {
            ++last_k;                                /* next prime index   */
            uint32_t p = get_prime(last_k);          /* prime(last_k)      */
            uint32_t r = p % 5;                      /* residue modulo 5  */
            if ((r & 1) == 0) {                      /* residue is even   */
                cache[cache_len++] = (seq_t)last_k;
            }
        }
    }

    return cache[n - 1];   /* 1‑based indexing for the sequence */
}
