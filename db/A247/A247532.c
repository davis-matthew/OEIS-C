/*
 * A247532  Number of length n+3 0..7 arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 21, u64 n <= 49, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A247532.
   Recurrence: a(n) is the number of length n+3 arrays over {0,…,7}
   such that in every consecutive four terms some two disjoint pairs
   have the same sum.  This is obtained by dynamic programming over
   the last three terms – the state space has size 8³ = 512 and
   transitions are allowed exactly when the new 4‑term window
   satisfies the pair‑sum property.  Memoisation keeps the values so
   that arbitrary calls for n ≥ 1 are answered in O(1) after the
   first call. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build; do not redeclare them. */

seq_t a(idx_t n)
{
    /* Persistent data across calls */
    static bool initialized = false;
    static seq_t dp_cur[512];
    static seq_t dp_next[512];
    static seq_t *cache = NULL;        /* cache[n] = a(n), 1‑based (cache[0] unused) */
    static size_t cache_capacity = 0;  /* allocated size of cache */
    static size_t max_cached = 0;      /* largest n computed so far */
    static idx_t cur_len = 3;          /* current array length represented by dp_cur */

    if (!initialized) {
        /* initialise all 3‑digit prefixes: each contributes 1 */
        for (int i = 0; i < 512; ++i)
            dp_cur[i] = (seq_t)1;
        for (int i = 0; i < 512; ++i)
            dp_next[i] = (seq_t)0;

        /* allocate cache for index 0 only */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache) cache[0] = (seq_t)0;
        cache_capacity = 1;
        max_cached = 0;
        initialized = true;
    }

    if (n < 1) return (seq_t)0;   /* offset 1 */

    /* extend DP until we have a(n) */
    while (max_cached < n) {
        /* new length */
        cur_len++;

        /* reset next DP */
        for (int i = 0; i < 512; ++i)
            dp_next[i] = (seq_t)0;

        /* transition from all current states */
        for (int i = 0; i < 512; ++i) {
            seq_t cur_val = dp_cur[i];
            if (cur_val == (seq_t)0) continue;

            int ai = i >> 6;                 /* high 2 bits */
            int bi = (i >> 3) & 7;           /* middle 3 bits */
            int ci = i & 7;                  /* low 3 bits */

            for (int d = 0; d < 8; ++d) {
                /* check pair‑sum condition for window (ai,bi,ci,d) */
                bool ok = ((ai + bi) == (ci + d)) ||
                          ((ai + ci) == (bi + d)) ||
                          ((ai + d)  == (bi + ci));
                if (ok) {
                    int next_index = (bi << 6) | (ci << 3) | d;
                    dp_next[next_index] += cur_val;
                }
            }
        }

        /* swap dp arrays */
        for (int i = 0; i < 512; ++i)
            dp_cur[i] = dp_next[i];

        /* total count for this length */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 512; ++i)
            total += dp_cur[i];

        /* n corresponding to this length (length = n+3) */
        idx_t cur_n = cur_len - 3;

        /* enlarge cache if necessary */
        if ((size_t)cur_n + 1 > cache_capacity) {
            size_t new_cap = (size_t)cur_n + 1;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;    /* allocation failure */
            /* initialise new slots (not strictly required) */
            for (size_t k = cache_capacity; k < new_cap; ++k)
                new_cache[k] = (seq_t)0;
            cache = new_cache;
            cache_capacity = new_cap;
        }

        cache[(size_t)cur_n] = total;
        if (cur_n > max_cached) max_cached = cur_n;
    }

    return cache[(size_t)n];
}
