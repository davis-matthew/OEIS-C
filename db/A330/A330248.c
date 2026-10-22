/*
 * A330248  a(1) = 1; for n > 1, a(n) is the least nonnegative number such
 *          that a(n) + a(n-1) + n is a prime number.
 * https://oeis.org/A330248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) is the smallest non‑negative integer
   such that a(n)+a(n‑1)+n is prime.  The sequence is computed on‑demand
   and cached for future calls.  See OEIS A330248. */

/* Required helper for primality testing */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the nth term of A330248 (1‑based index). */
seq_t a(idx_t n)
{
    /* Special case for the first term. */
    if (n == 1)
        return (seq_t)1;

    /* Static cache for computed terms. */
    static seq_t *cache   = NULL;
    static size_t  size   = 0;   /* number of elements stored (>=1) */
    static size_t  cap    = 0;   /* capacity of cache array          */

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cap = 4;                                 /* initial capacity   */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)                                 /* allocation failed  */
            return 0;
        cache[0] = (seq_t)1;                       /* a(1) */
        cache[1] = (seq_t)0;                       /* a(2) */
        size    = 2;                               /* two terms stored   */
    }

    /* Grow cache if required. */
    if ((size_t)n > cap) {
        while (cap < (size_t)n)
            cap <<= 1;                         /* double the size */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!cache)                                /* allocation failed  */
            return 0;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= size)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t k = (idx_t)(size + 1); k <= n; ++k) {
        seq_t prev     = cache[k - 2];         /* a(k-1)            */
        seq_t candidate = 0;                   /* a(k) candidate   */
        /* Find the smallest non‑negative candidate. */
        while (1) {
            seq_t sum = candidate + prev + (seq_t)k;
            if (is_prime_big(sum))
                break;
            ++candidate;
        }
        cache[k - 1] = candidate;              /* store a(k)        */
    }

    size = n;
    return cache[n - 1];
}
