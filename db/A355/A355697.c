/*
 * A355697  a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-1) + g - 1 if a(n-1) is
 *          prime, otherwise a(n) = a(n-1) + g + 1, where g = a(n-1) - a(n-2).
 * https://oeis.org/A355697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1.
   For n>1 let g = a(n-1) - a(n-2);
   If a(n-1) is prime then a(n)=a(n-1)+g-1,
   otherwise a(n)=a(n-1)+g+1. */

#include <stdlib.h>
#include <stdbool.h>

/* Small trial‑division prime test for seq_t values.  It is adequate
   for the sizes encountered in this sequence (a(n) < 10⁹ for n≤10000). */
static bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;

    for (idx_t i = 3; (seq_t)i <= n / (seq_t)i; i += 2) {
        if (n % (seq_t)i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries in cache */

    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* initialise the cache on first use */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – unlikely */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* extend cache up to the requested index, if necessary */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* reallocation failure – unlikely */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev     = cache[i - 1];
            seq_t prevprev = cache[i - 2];
            seq_t g        = prev - prevprev;   /* g = a(n-1)-a(n-2) */

            g = is_prime_seq(prev) ? (g - 1) : (g + 1);
            cache[i] = prev + g;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
