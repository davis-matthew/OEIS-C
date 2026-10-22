/*
 * A318620  a(0) = a(1) = 1; for n > 1, a(n) = Sum_{k=0..n-2} a(k) AND
 *          a(n-k-2).
 * https://oeis.org/A318620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 197, u32 n <= 1262, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A318620
   a(0) = a(1) = 1
   a(n) = Σ_{k=0}^{n-2} a(k) AND a(n-k-2),  for n ≥ 2
   (bitwise AND, integer addition) */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation table */
static size_t cache_len = 0;     /* number of stored terms */

static void ensure_cache(idx_t n)
{
    /* Initialise the cache if necessary */
    if (cache_len == 0) {
        cache_len = 2;                          /* indices 0 and 1 are needed */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = cache[1] = (seq_t)1;
    }

    /* Extend the cache to contain a(n) */
    if ((size_t)n + 1 > cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;

        /* Compute all new entries */
        for (size_t i = old_len; i <= new_len - 1; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= (idx_t)i - 2; ++k) {
                /* a(k) AND a(i-k-2) */
                sum += cache[(size_t)k] & cache[(size_t)(i - k - 2)];
            }
            cache[i] = sum;
        }
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
