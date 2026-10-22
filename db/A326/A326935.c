/*
 * A326935  a(1) = 1; a(n) = the largest prime factor of a(n-1) if a(n-1) is a
 *          composite number, otherwise a(n)=a(n-1)+n.
 * https://oeis.org/A326935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326935
   a(1) = 1;
   a(n) = a(n-1) + n                if a(n-1) is 1 or prime,
   a(n) = largest_prime_factor(a(n-1))   otherwise (a(n-1) composite)
   The function memoises results to handle arbitrary call order.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/largest_prime_divisor.h"

/* cache for memoised values */
static seq_t *cache = NULL;       /* array of a(k), 0‑based (a(1) -> cache[0]) */
static size_t cache_len = 0;      /* number of computed terms */
static size_t cache_cap = 0;      /* allocated capacity */

/* ensure the cache can hold at least n terms */
static void ensure_cache_cap(size_t n)
{
    if (cache_cap >= n)
        return;
    /* increase capacity by doubling until adequate */
    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap < n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* return a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) { /* offset starts at 1, but guard for safety */
        return (seq_t)0;
    }

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* initialise a(1) */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
        if (n == 1)
            return cache[0];
    }

    /* compute forward if necessary */
    if (n > cache_len) {
        ensure_cache_cap((size_t)n);
        for (size_t idx = cache_len + 1; idx <= n; ++idx) {
            seq_t prev = cache[idx - 2]; /* a(idx-1) */
            bool composite;

            if (prev == (seq_t)1) {
                composite = false;          /* 1 is not composite */
            } else if (is_prime_big(prev)) {
                composite = false;          /* prime */
            } else {
                composite = true;           /* composite */
            }

            seq_t cur;
            if (composite) {
                /* largest prime factor – safe cast: all terms fit in uint64_t */
                uint64_t prev_u = (uint64_t)prev;
                uint64_t lpf = largest_prime_divisor(prev_u);
                cur = (seq_t)lpf;
            } else {
                cur = prev + (seq_t)idx;     /* index is idx_t (uint64_t) */
            }
            cache[idx - 1] = cur;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
