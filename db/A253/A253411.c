/*
 * A253411  Indices of centered octagonal numbers (A016754) which are also
 *          centered pentagonal numbers (A005891).
 * https://oeis.org/A253411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1442*a(n-2) - 1442*a(n-3) - a(n-4) + a(n-5)
 *            Initial values (for n = 1 … 5): 1, 76, 646, 108871, 930811
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free, size_t */

static seq_t *cache      = NULL; /* cached terms, zero‑based (cache[0] = a(1)) */
static size_t cache_cap  = 0;    /* allocated capacity in terms   */
static idx_t  cache_len  = 0;    /* number of terms currently stored */

/* Public function: returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, 0 is unused */
        return 0;

    /* initialise cache with the first five terms on first call */
    if (cache_len == 0) {
        cache_cap = 8;               /* start with a small, but sufficient, size */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure -> arbitrary 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)76;
        cache[2] = (seq_t)646;
        cache[3] = (seq_t)108871;
        cache[4] = (seq_t)930811;
        cache_len = 5;
    }

    /* extend the cache up to the requested index n */
    while (cache_len < n) {
        /* grow capacity if needed (double size) */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_arr) return 0;   /* allocation failure -> arbitrary 0 */
            for (size_t i = 0; i < cache_len; ++i)
                new_arr[i] = cache[i];
            free(cache);
            cache     = new_arr;
            cache_cap = new_cap;
        }

        /* compute a(cache_len + 1) using the recurrence */
        /* cache_len is the number of terms already stored; the next
           term will have index cache_len + 1 (zero‑based index = cache_len) */
        seq_t t1 = cache[cache_len - 1]; /* a(n-1)   */
        seq_t t2 = cache[cache_len - 2]; /* a(n-2)   */
        seq_t t3 = cache[cache_len - 3]; /* a(n-3)   */
        seq_t t4 = cache[cache_len - 4]; /* a(n-4)   */
        seq_t t5 = cache[cache_len - 5]; /* a(n-5)   */

        seq_t next = t1
                   + t2 * (seq_t)1442
                   - t3 * (seq_t)1442
                   - t4
                   + t5;

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
