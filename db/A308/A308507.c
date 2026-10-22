/*
 * A308507  a(n) = (a(n-1) + a(n-2))^4, for n >= 2; a(0)=0, a(1)=1.
 * https://oeis.org/A308507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (a(n-1) + a(n-2))^4, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;    /* array of cached values */
    static size_t  cap    = 0;       /* allocated capacity */
    static idx_t   up_to  = (idx_t)-1;  /* highest index computed so far */

    /* Ensure the cache can hold indices 0 … n */
    if (!cache || (size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first two terms if this is the first usage */
    if (up_to == (idx_t)-1) {
        cache[0] = 0;
        if (cap > 1) cache[1] = 1;
        up_to = (cap > 1) ? 1 : 0;          /* highest computed index */
    }
    /* If we haven't stored a(1) yet but it is required, do it now */
    else if (up_to < 1 && n >= 1) {
        cache[1] = 1;
        up_to = 1;
    }

    /* Compute any missing terms up to n */
    idx_t start = up_to + 1;
    if (start < 2) start = 2;                /* recurrence starts at n=2 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2];
        seq_t sq  = sum * sum;               /* (a+b)^2 */
        cache[i]  = sq * sq;                 /* (a+b)^4 */
    }
    if (n > up_to) up_to = n;

    return cache[n];
}
