/*
 * A331840  Numbers k such that 30*k-13, 30*k-11 are twin primes.
 * https://oeis.org/A331840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that 30*k-13 and 30*k-11 are twin
 *            primes
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache of discovered terms and extends it only as needed.
   The next integer k is tested until enough terms are found.  Prime testing is performed with
   the provided is_prime_big() routine.  The cache grows geometrically to keep amortized
   time per call very small.  The algorithm is efficient enough for n up to about 10 000
   (k stays below 1 million). */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of terms */
    static seq_t *cache = NULL;
    static size_t   cache_len = 0;   /* number of terms stored */
    static size_t   cache_cap = 0;   /* capacity of the cache array */
    static idx_t    next_k   = 1;    /* next k to test */

    if (n < 1) return 0;   /* undefined for n=0, but safety */

    /* if we already have the term, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache until we have n terms */
    while (cache_len < n) {
        idx_t k = next_k;
        ++next_k;

        /* compute the two candidate primes */
        seq_t p1 = (seq_t)(30 * k - 13);
        seq_t p2 = (seq_t)(30 * k - 11);

        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* need more room in cache? */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
