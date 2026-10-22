/*
 * A344496  a(0)=0; for n > 0, a(n) = a(n-1)*n + n if n is odd, (a(n-1) + n)*n
 *          otherwise.
 * https://oeis.org/A344496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0,
   a(n) = a(n-1)*n + n      if n is odd
   a(n) = (a(n-1)+n)*n      if n is even */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of previously computed terms   */
    static idx_t cache_size = 0;     /* number of valid entries in cache     */
    static size_t cache_cap = 0;      /* allocated capacity of the cache      */

    /* Ensure the cache can hold at least n+1 entries          */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;            /* initial small size      */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* out‑of‑memory failure   */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the zeroth element if we haven't computed any term yet */
    if (cache_size == 0) {
        cache[0] = 0;
        cache_size = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t term;
        if (i & 1) {                     /* i is odd */
            term = prev * (seq_t)i + (seq_t)i;
        } else {                         /* i is even */
            term = (prev + (seq_t)i) * (seq_t)i;
        }
        cache[i] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
