/*
 * A281292  Squarefree numbers that are the sum of two squares in exactly one
 *          way.
 * https://oeis.org/A281292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 5005, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281292: squarefree numbers that are the sum of two squares in exactly one way.
   They are: 1, 2, all primes p ≡ 1 (mod 4), and all numbers 2p where p is a prime ≡ 1 (mod 4). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Offset starts at 1.  For n == 0 we return 0 (unused). */
    if (n == 0) return 0;

    /* Lazily generated cache that remembers already computed terms. */
    static seq_t *cache = NULL;          /* array of terms 1 .. cache_size (1‑based) */
    static idx_t cache_size = 0;         /* number of terms currently stored */
    static size_t cache_capacity = 0;    /* allocated array size */
    static idx_t last_tested = 0;        /* largest integer that has been examined */

    /* Return directly if we already have the term. */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure (unlikely) */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Generate terms until we reach the requested index. */
    idx_t pos = cache_size;            /* next position to fill, 0‑based */
    idx_t m = last_tested + 1;         /* next integer to test */

    while (pos < n) {
        bool in_seq = false;

        if (m == 1 || m == 2) {
            in_seq = true;
        } else if ((m & 1) == 0) {     /* even → candidate 2p */
            idx_t half = m >> 1;
            if ((half & 3) == 1 && is_prime(half))
                in_seq = true;
        } else {                       /* odd → candidate prime p */
            if ((m & 3) == 1 && is_prime(m))
                in_seq = true;
        }

        if (in_seq) {
            cache[pos++] = (seq_t)m;
        }

        last_tested = m;
        ++m;
    }

    cache_size = n;
    return cache[n - 1];
}
