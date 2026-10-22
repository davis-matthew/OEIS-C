/*
 * A321232  Length of n-th term of A321225.
 * https://oeis.org/A321232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) + 5 (n > 9) Initial values: a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=3, a(5)=2, a(6)=4, a(7)=4, a(8)=4, a(9)=5
 * Range:     u8 n <= 459, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_values[10] = {0, 1, 1, 1, 3, 2, 4, 4, 4, 5};

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of computed terms, 0..cur_max */
    static idx_t cache_size  = 0;       /* allocated size of cache            */
    static idx_t cur_max     = 0;       /* highest index computed so far       */

    /* First call: allocate storage big enough for at least the first 10
       terms and for the requested index n.  The initial size may be
       larger than needed but keeps allocation distances small. */
    if (cache == NULL) {
        cache_size = (n + 1 >= 10) ? n + 1 : 10;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) abort();

        /* Initialise indices 0..9.  For indices beyond n, the
           values are not yet needed. */
        for (idx_t i = 0; i <= 9; ++i)
            cache[i] = base_values[i];

        cur_max = 9;
    }

    /* Grow the buffer if the requested index is beyond current size. */
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;   /* double to amortise future growth */
        if (new_size < 10) new_size = 10;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = new_size;

        /* Initialise any newly allocated slots up to index 9 with
           the known base values.  (If 9 is already covered, nothing
           new is done.) */
        for (idx_t i = cur_max + 1; i <= 9 && i < cache_size; ++i)
            cache[i] = base_values[i];

        cur_max = (cur_max < 9) ? 9 : cur_max;
    }

    /* Compute terms that have not yet been computed, up to index n. */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        if (i <= 9) {
            /* base values are already in cache; nothing to do */
            continue;
        }
        cache[i] = cache[i - 9] + (seq_t)5;
    }
    if (n > cur_max) cur_max = n;

    return cache[n];
}
