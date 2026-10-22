/*
 * A377371  a(n) = k*(a(n-1)+n), k=-1 for prime n, otherwise k=1 (a(1)=1).
 * https://oeis.org/A377371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + n for composite n, else -(a(n-1)+n). a(1)=1
 * Range:     i8 n <= 35, i16 n <= 1030, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Cache for memoisation of sequence values (0‑based: cache[0] = a(1)) */
static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of elements currently stored */
static size_t cache_cap = 0; /* allocated capacity */

/* Ensure the cache can hold at least `n` elements (1‑based index) */
static void ensure_cache_capacity(size_t n)
{
    if (n <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 1;
    if (newcap < n) newcap = n;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr) abort();  /* allocation failure */
    cache = newptr;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined; offset is 1 */

    size_t target = (size_t)n;            /* number of elements needed */
    if (target > cache_len) {
        ensure_cache_capacity(target);

        /* Build missing terms up to a(n) */
        for (size_t i = cache_len; i < target; ++i) {
            if (i == 0) {  /* a(1) */
                cache[0] = (seq_t)1;
            } else {
                idx_t k = (idx_t)(i + 1); /* current n */
                seq_t prev = cache[i - 1];
                seq_t val = prev + (seq_t)k;
                if (is_prime(k))
                    val = -val;
                cache[i] = val;
            }
        }
        cache_len = target;
    }
    return cache[n - 1];
}
