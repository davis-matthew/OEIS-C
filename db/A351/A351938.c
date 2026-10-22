/*
 * A351938  a(0) = 1; a(n) = -a(n-1) + 2 * Sum_{k=0..floor((n-1)/2)} a(k) *
 *          a(n-2*k-1).
 * https://oeis.org/A351938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = –a(n–1) + 2 * Σ_{k=0..⌊(n–1)/2⌋} a(k)·a(n–2k–1)
 *            for n ≥ 1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation ------------------------------------- */
    static seq_t *cache = NULL;   /* array of previously computed terms  */
    static size_t cap   = 0;      /* current allocated size (≥1)        */

    /* initialise cache on first call ----------------------------------- */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));   /* allocate one element   */
        if (cache == NULL) abort();              /* out of memory          */
        cache[0] = 1;                            /* a(0) = 1               */
        cap = 1;
    }

    /* extend the cache if necessary ------------------------------------- */
    if (cap <= (size_t)n) {
        size_t oldcap = cap;
        size_t newcap = (size_t)n + 1;

        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();                /* out of memory          */
        cache = tmp;
        /* the new slots are left uninitialised – they will be filled
           below.                                                     */
        cap = newcap;

        /* compute every term that is still missing --------------------- */
        for (idx_t cur = (idx_t)oldcap; cur <= n; ++cur) {
            seq_t sum = 0;
            idx_t bound = (cur - 1) / 2;          /* ⌊(cur-1)/2⌋            */
            for (idx_t k = 0; k <= bound; ++k) {
                sum = sum + cache[k] * cache[cur - 2 * k - 1];
            }
            cache[cur] = (sum << 1) - cache[cur - 1];
        }
    }

    return cache[n];
}
