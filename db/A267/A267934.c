/*
 * A267934  Binary representation of the n-th iteration of the "Rule 249"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2) (for n > 4) Recurrence starts
 *            with a(0)=1, a(1)=1, a(2)=11011, a(3)=1111111, a(4)=111111111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t cache_len = 0;          /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if (cache_cap >= (size_t)needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)needed)
        new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* memory allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        /* Expand cache to store up to n. */
        ensure_cache_capacity(n + 1);

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)           cache[i] = 1;
            else if (i == 1)      cache[i] = 1;
            else if (i == 2)      cache[i] = 11011;
            else if (i == 3)      cache[i] = 1111111;
            else if (i == 4)      cache[i] = 111111111;
            else                  /* recurrence for n > 4 */
                cache[i] = 101 * cache[i-1] - 100 * cache[i-2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
