/*
 * A266508  Binary representation of the n-th iteration of the "Rule 28"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((100*10^n + 89)/99) (OEIS A266508)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms      */
static idx_t  cache_len = 0;     /* number of terms stored       */

static void ensure_cache(idx_t n)
{
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();             /* no memory – abort          */
        cache = tmp;

        /* compute missing terms, using the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)11;
            } else if (i == 2) {
                cache[i] = (seq_t)101;
            } else {
                cache[i] = (seq_t)10 * cache[i-1] + cache[i-2]
                           - (seq_t)10 * cache[i-3];
            }
        }
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
