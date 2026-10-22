/*
 * A353778  a(1)=1. If a(n) is a novel term (seen for first time), a(n+1) =
 *          number of prior terms a(j) > a(n), 1 <= j <= n-1. If a(n) is a
 *          repeat of a term last seen at a(m), m < n, then a(n+1) = n-m-1.
 * https://oeis.org/A353778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 387, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   For n > 1:
     If a(n) is a novel term (has not appeared before), then
         a(n+1) = # { j | 1 ≤ j ≤ n-1 and a(j) > a(n) }.
     If a(n) is a repeat of a term last seen at a(m) with m < n,
         then a(n+1) = n - m - 1. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *cache = NULL;          /* a(i) values, 1-indexed */
    static size_t cache_cap = 0;         /* capacity of cache */
    static size_t cache_len = 0;         /* highest computed index, >=1 when ready */

    /* arrays that map a value v to its last two occurrence indices:
       last[v]   = most recent index where v appeared
       prev_last[v] = previous index before last[v] (0 if none) */
    static idx_t *last = NULL;
    static idx_t *prev_last = NULL;
    static size_t occ_cap = 0;           /* capacity of last/prev_last arrays */

    /* helper: ensure cache has capacity for index up to n */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1U;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* helper: ensure last/prev_last can address values up to n (values ≤ n) */
    if ((size_t)n > occ_cap) {
        size_t new_cap = occ_cap ? occ_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1U;
        idx_t *tmp1 = (idx_t *)realloc(last, (new_cap + 1) * sizeof(idx_t));
        idx_t *tmp2 = (idx_t *)realloc(prev_last, (new_cap + 1) * sizeof(idx_t));
        last = tmp1;
        prev_last = tmp2;
        for (size_t i = occ_cap + 1; i <= new_cap; ++i) {
            last[i] = 0;
            prev_last[i] = 0;
        }
        occ_cap = new_cap;
    }

    /* initialization: compute a(1) if not done yet */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache_len = 1;
        /* ensure arrays can address value 1 */
        if (occ_cap < (size_t)1) {
            size_t new_cap = 1;
            idx_t *tmp1 = (idx_t *)realloc(last, (new_cap + 1) * sizeof(idx_t));
            idx_t *tmp2 = (idx_t *)realloc(prev_last, (new_cap + 1) * sizeof(idx_t));
            last = tmp1;
            prev_last = tmp2;
            for (size_t i = occ_cap + 1; i <= new_cap; ++i) {
                last[i] = 0;
                prev_last[i] = 0;
            }
            occ_cap = new_cap;
        }
        idx_t val_idx = (idx_t)1;
        last[val_idx] = 1;
        prev_last[val_idx] = 0;
    }

    /* compute all terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* a(i) depends on a(i-1) */
        seq_t prev_val = cache[i - 1];
        idx_t idx_prev = (idx_t)prev_val;
        idx_t last_idx = last[idx_prev];        /* should be i-1 */
        idx_t prev_occ = prev_last[idx_prev];   /* previous occurrence before i-1 */

        seq_t next_val;
        if (prev_occ == 0) {
            /* novel term: count prior terms > prev_val */
            seq_t cnt = 0;
            for (idx_t j = 1; j <= (idx_t)i - 2; ++j) {
                if (cache[j] > prev_val) ++cnt;
            }
            next_val = cnt;
        } else {
            /* repeat term: a(i) = (i-1) - prev_occ - 1 */
            next_val = (seq_t)((i - 1) - prev_occ - 1);
        }

        cache[i] = next_val;

        /* record occurrence of the new value next_val */
        idx_t idx_new = (idx_t)next_val;
        if ((size_t)idx_new > occ_cap) {
            size_t new_cap = occ_cap ? occ_cap : 1;
            while (new_cap < (size_t)idx_new) new_cap <<= 1U;
            idx_t *tmp1 = (idx_t *)realloc(last, (new_cap + 1) * sizeof(idx_t));
            idx_t *tmp2 = (idx_t *)realloc(prev_last, (new_cap + 1) * sizeof(idx_t));
            last = tmp1;
            prev_last = tmp2;
            for (size_t j = occ_cap + 1; j <= new_cap; ++j) {
                last[j] = 0;
                prev_last[j] = 0;
            }
            occ_cap = new_cap;
        }
        prev_last[idx_new] = last[idx_new];
        last[idx_new] = i;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
