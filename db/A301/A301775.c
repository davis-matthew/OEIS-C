/*
 * A301775  Number of odd chordless cycles in the (2n+1)-web graph.
 * https://oeis.org/A301775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301775: Number of odd chordless cycles in the (2n+1)-web graph
   Recurrence (for n > 5):
     a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - a(n-4)
   Initial values:
     a(1) = 0
     a(2) = 12
     a(3) = 30
     a(4) = 74
     a(5) = 200
*/

#include <stdlib.h> /* for malloc, realloc, free */

/*--------- static storage for memoisation ----------*/
static seq_t *cache     = NULL;   /* 1‑based, cache[0] unused */
static idx_t cache_size = 0;     /* allocated size (number of slots)  */
static idx_t max_comp   = 0;     /* largest n that has been computed  */

/*--------- helper to grow the cache if necessary ----------*/
static void grow_cache(idx_t need)
{
    if (need < cache_size) return;

    size_t new_size = (cache_size > 0) ? cache_size : 16;
    while (new_size <= (size_t)need) new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    /* zero‑initialize the newly allocated part */
    for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;

    cache = tmp;
    cache_size = new_size;
}

/*--------- main sequence function ----------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined below offset */

    /* initialise cache on first call, with the known initial terms */
    if (!cache) {
        grow_cache(5);                /* reserve room for at least the first 5 terms */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)30;
        cache[4] = (seq_t)74;
        cache[5] = (seq_t)200;
        max_comp = 5;
    }

    /* return cached value if already known */
    if (n <= max_comp) return cache[n];

    /* compute all missing terms up to n using the recurrence */
    grow_cache(n);
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i - 1];
        val += cache[i - 2];
        val += (seq_t)2 * cache[i - 3];
        val -= cache[i - 4];
        cache[i] = val;
    }
    max_comp = n;

    return cache[n];
}
