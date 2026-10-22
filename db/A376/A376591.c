/*
 * A376591  Inflection and undulation points in the sequence of squarefree
 *          numbers (A005117).
 * https://oeis.org/A376591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_squarefree, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) contains the indices k-2 at which the second
   difference of the squarefree number sequence (A005117) is zero.
   The sequence starts with a(1)=1, a(2)=4, a(3)=9, ... */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t x);       */
#include "../utils/seq_alloc.h"      /* seq_t* seq_alloc(size_t);          */
#include "../utils/seq_realloc.h"   /* seq_t* seq_realloc(seq_t*, size_t, size_t); */

seq_t a(idx_t n)
{
    /* cache of found values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* state for scanning squarefree numbers */
    static idx_t last_processed = 1;   /* next integer to test */
    static idx_t sqf_seen = 0;         /* how many squarefree numbers seen */
    static seq_t prev2 = 0;
    static seq_t prev1 = 0;

    if (n == 0)          /* offset is 1; behaviour below 1 is irrelevant */
        return (seq_t)0;

    /* Extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Skip non‑squarefree numbers */
        if (!is_squarefree((seq_t)last_processed)) {
            ++last_processed;
            continue;
        }

        /* New squarefree number encountered */
        ++sqf_seen;
        seq_t cur = (seq_t)last_processed;

        if (sqf_seen >= 3) {
            /* second difference: s_k - 2*s_{k-1} + s_{k-2} */
            seq_t e = cur - (seq_t)2 * prev1 + prev2;
            if (e == (seq_t)0) {
                /* store k-2 */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();          /* unrecoverable */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)(sqf_seen - 2);
            }
        }

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
        ++last_processed;
    }

    return cache[(size_t)(n - 1)];
}
