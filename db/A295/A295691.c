/*
 * A295691  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 2, a(2) =
 *          2, a(3) = 1.
 * https://oeis.org/A295691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) initial values: a(0) = 2, a(1) =
 *            2, a(2) = 2, a(3) = 1
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function returns the n‑th term of OEIS A295691.
   It uses a growing static cache so that any order of
   arguments works and calls are fast.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of cached terms              */
    static idx_t  cached_n = 0;   /* largest index presently computed  */

    /* initial cache with the first four terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */

        cache[0] = 2;
        cache[1] = 2;
        cache[2] = 2;
        cache[3] = 1;
        cached_n = 3;
    }

    /* any requested value already known? */
    if (n <= cached_n) {
        return cache[n];
    }

    /* grow the cache to accommodate indices up to n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (cache == NULL) abort();              /* allocation failure */

    /* compute and store new terms using the recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 4];
    }

    cached_n = n;
    return cache[n];
}
