/*
 * A291141  a(1) = 3 and a(n+1) is the least prime greater than a(n) + 2n such
 *          that a(n+1) + 2n + 2 is prime.
 * https://oeis.org/A291141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 214, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A291141: a(1) = 3
   For n ≥ 1, a(n+1) is the least prime > a(n)+2n such that
   a(n+1)+2n+2 is also prime.  Computed by brute‑force search with
   a growing cache. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers supplied by the build system */
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;        /* cached a(n) values, 0‑based */
static idx_t    cache_size = 0;    /* current allocated size   */
static idx_t    max_computed = 0;  /* number of terms stored   */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Initialise cache with a(1)=3 */
    if (max_computed == 0) {
        cache_size   = 1;
        cache        = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)3;
        max_computed = 1;
    }

    /* Grow cache if necessary */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) {
            free(cache);
            cache    = NULL;
            cache_size = 0;
            max_computed = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = n;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed; i < n; ++i) {
        seq_t prev = cache[i - 1];               /* a(i)   */
        seq_t start = prev + (seq_t)(2 * i);     /* a(i)+2i */
        seq_t p = (seq_t)next_prime((uint64_t)start);   /* smallest prime > start */

        /* Ensure p + 2i + 2 is also prime */
        while (!is_prime_big(p + (seq_t)(2 * i) + 2)) {
            p = (seq_t)next_prime((uint64_t)p);
        }
        cache[i] = p;                           /* a(i+1) */
    }

    max_computed = n;
    return cache[n - 1];
}
