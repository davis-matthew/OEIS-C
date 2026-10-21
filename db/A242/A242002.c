/*
 * A242002  Sum_{k=1..n} (-1)^isprime(k)*2^k.
 * https://oeis.org/A242002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} (-1)^{isprime(k)} * 2^k
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 31, i64 n <= 62, i128 n <= 127,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* We keep a static cache of cumulative sums. The array is 1‑based;
       cache[0] is defined as 0 for the recurrence.  */
    static seq_t *cache = NULL;      /* array of a(i) values */
    static idx_t cache_size = 0;     /* highest n cached so far */

    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

    /* Allocate or grow the cache as necessary (size n+1 to use 1‑based indexing) */
    if (n > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();   /* out‑of‑memory, terminate */
        cache = new_cache;

        /* If cache was empty, initialise index 0 */
        if (cache_size == 0)
            cache[0] = 0;

        /* Compute new terms incrementally */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t term = pow2(i);          /* 2^i */
            if (is_prime(i))
                cache[i] = cache[i - 1] - term;
            else
                cache[i] = cache[i - 1] + term;
        }
        cache_size = n;
    }
    return cache[n];
}
