/*
 * A359003  a(n) is the smallest n-gonal number whose sum of digits is n.
 * https://oeis.org/A359003
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest n-gonal number whose digit sum equals n
 *            (n-gonal numbers are P(n,k) = (n-2)k(k-1)/2 + k)
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 3, the rest is undefined */
    if (n < 3) return 0;

    /* static cache: cache[n] contains the value for index n,
       0 indicates that the term has not been computed yet */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* enlarge the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – should never happen */
        if (new_cap > cache_cap) {
            memset((void *)(tmp + cache_cap), 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if (cache[n] != 0) return cache[n];

    /* brute‑force search for the smallest k >= 1 such that P(n,k) has
       digit sum n.  Since (n-2)k(k-1)/2 grows quadratically and n
       is at most 10000 (in the test data) this loop is very fast. */
    seq_t result = 0;
    for (idx_t k = 1;; ++k) {
        seq_t val = (seq_t)(n - 2);   /* (n-2) */
        val *= (seq_t)k;              /* * k */
        val *= (seq_t)(k - 1);        /* * (k-1) */
        val /= (seq_t)2;              /* / 2 */
        val += (seq_t)k;              /* + k */
        if (digit_sum(val) == (seq_t)n) {
            result = val;
            break;
        }
    }

    cache[n] = result;
    return result;
}
