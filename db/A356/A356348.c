/*
 * A356348  a(0) = 0; for n > 0, a(n) is the number of preceding terms having
 *          the same digit sum as a(n-1).
 * https://oeis.org/A356348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n > 0, a(n) = the number of preceding terms having
 *            the same digit sum as a(n-1). Preceding means indices j with 0
 *            <= j < n
 * Range:     u8 n <= 2596, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/ensure_cache.h"

#ifdef __cplusplus
#include <map>
#endif

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;          /* sequence values            */
    static size_t cache_cap = 0;         /* allocated capacity (elements) */
    static idx_t computed_upto = 0;      /* largest index already computed (∈ [0,n) ) */

#ifdef __cplusplus
    static std::map<seq_t, size_t> counts_map;   /* digit‑sum → occurrence count */
#else
    /* For plain C we use a simple dynamic array of key/value pairs. */
    struct ds_entry { seq_t key; size_t count; };
    static struct ds_entry *counts_arr = NULL;
    static size_t counts_sz   = 0;       /* number of entries used   */
    static size_t counts_cap  = 0;       /* allocated entries        */
#endif

    /* ---- initialise once ---- */
    if (cache == NULL) {
        /* allocate at least one slot for a[0] */
        ensure_cache(1, &cache, &cache_cap);
        cache[0] = 0;
        computed_upto = 0;
#ifdef __cplusplus
        counts_map[0] = 1;
#else
        counts_cap = 1;
        counts_arr = (struct ds_entry*)malloc(counts_cap * sizeof(struct ds_entry));
        counts_arr[0].key   = 0;
        counts_arr[0].count = 1;
        counts_sz = 1;
#endif
    }

    if (n == 0) return cache[0];

    /* ---- grow cache if necessary ---- */
    if ((size_t)n >= cache_cap) {
        ensure_cache((idx_t)n + 1, &cache, &cache_cap);
    }

    /* ---- compute missing elements ---- */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t  prev    = cache[i - 1];
        seq_t  ds_prev = digit_sum(prev);

        size_t cnt = 0;
#ifdef __cplusplus
        auto it = counts_map.find(ds_prev);
        if (it != counts_map.end()) cnt = it->second;
#else
        size_t j;
        for (j = 0; j < counts_sz; ++j)
            if (counts_arr[j].key == ds_prev) break;
        if (j < counts_sz) cnt = counts_arr[j].count;
#endif
        cache[i] = (seq_t)cnt;   /* cast is safe: cnt <= n <= 10000 */

        seq_t ds_new = digit_sum(cache[i]);

#ifdef __cplusplus
        ++counts_map[ds_new];
#else
        /* update counts for ds_new */
        size_t k;
        for (k = 0; k < counts_sz; ++k)
            if (counts_arr[k].key == ds_new) break;
        if (k < counts_sz) {
            ++counts_arr[k].count;
        } else {
            /* add new entry */
            if (counts_sz == counts_cap) {
                counts_cap = (counts_cap == 0 ? 1 : counts_cap * 2);
                counts_arr = (struct ds_entry*)realloc(
                    counts_arr,
                    counts_cap * sizeof(struct ds_entry));
            }
            counts_arr[counts_sz].key   = ds_new;
            counts_arr[counts_sz].count = 1;
            ++counts_sz;
        }
#endif
    }

    computed_upto = n;
    return cache[n];
}
