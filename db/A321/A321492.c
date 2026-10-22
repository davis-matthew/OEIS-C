/*
 * A321492  Numbers that can be written as (x + y)(x^2 + y^2), x > y > 0, in
 *          at least two ways.
 * https://oeis.org/A321492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A321492: numbers that can be written as (x + y)(x² + y²) with x > y > 0
 *  in at least two different ways.
 *  The implementation precomputes all values up to a growing bound
 *  and extracts those that occur at least twice.
 *  The cache is built lazily; any call to a(n) enlarges the cache
 *  until n terms are available.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Compute (x+y)(x²+y²)/2 for given s=x+y and d=x−y (or y−x). */
static inline seq_t
calc_A(idx_t s, idx_t d)
{
    unsigned __int128 ss = (unsigned __int128)s;
    unsigned __int128 dd = (unsigned __int128)d;
    unsigned __int128 val = ss * ((ss * ss) + (dd * dd)) / 2;
    return (seq_t)val;
}

/* Cached data ---------------------------------------------------------- */

static idx_t S_MAX = 0;     /* current upper bound for s = x+y */

#ifdef __cplusplus
static std::vector<seq_t> all_vals;
static std::vector<seq_t> dup_vals;
#else
static seq_t *all_vals  = NULL;
static seq_t *dup_vals  = NULL;
static size_t all_len   = 0;
static size_t dup_len   = 0;
#endif

/* Build or rebuild the cache until it contains at least 'needed' terms. */
static void
build_cache(idx_t needed)
{
#ifdef __cplusplus
    if (dup_vals.size() >= needed)
        return;                           /* already sufficient */

    /* free previous all_vals (not needed after the first loop) */
    all_vals.clear();
    dup_vals.clear();

    /* start with a modest bound and double until enough terms */
    if (S_MAX == 0)
        S_MAX = 2000;
    else
        S_MAX *= 2;

    while (true) {
        all_vals.reserve((size_t)S_MAX * (S_MAX + 1) / 4 + 10);

        /* generate all values for 2 ≤ s ≤ S_MAX, d parity same as s */
        for (idx_t s = 2; s <= S_MAX; ++s) {
            idx_t d_start = (s & 1u) ? 1 : 2;
            for (idx_t d = d_start; d < s; d += 2)
                all_vals.push_back(calc_A(s, d));
        }

        /* sort to find duplicates */
        std::sort(all_vals.begin(), all_vals.end(),
                  [](const seq_t &a, const seq_t &b) {
                      return cmp_seq_t(&a, &b) < 0;
                  });

        /* collect values appearing ≥ 2 times */
        for (size_t i = 0; i < all_vals.size(); ) {
            size_t j = i + 1;
            while (j < all_vals.size() && all_vals[j] == all_vals[i])
                ++j;
            if (j - i >= 2)
                dup_vals.push_back(all_vals[i]);
            i = j;
        }

        if (dup_vals.size() >= needed)
            break;                  /* sufficient */
        /* otherwise increase bound */
        all_vals.clear();
        dup_vals.clear();
        S_MAX *= 2;
    }
#else
    /* C implementation ---------------------------------------------- */
    /* If we already have enough terms, just return. */
    if (dup_len >= needed)
        return;

    /* free previous buffers */
    free(all_vals);
    free(dup_vals);
    all_vals = NULL;
    dup_vals = NULL;
    all_len = dup_len = 0;

    /* start with a modest bound and double until enough terms */
    if (S_MAX == 0)
        S_MAX = 2000;
    else
        S_MAX *= 2;

    while (true) {
        /* estimate number of pairs; add a small safety margin */
        size_t pair_count = (size_t)S_MAX * (S_MAX + 1) / 4 + 10;
        all_vals = (seq_t *)malloc(pair_count * sizeof(seq_t));
        if (!all_vals)
            return; /* allocation failed – unlikely for contest limits */

        /* generate values */
        size_t idx = 0;
        for (idx_t s = 2; s <= S_MAX; ++s) {
            idx_t d_start = (s & 1u) ? 1 : 2;
            for (idx_t d = d_start; d < s; d += 2)
                all_vals[idx++] = calc_A(s, d);
        }
        all_len = idx;

        /* sort */
        qsort(all_vals, all_len, sizeof(seq_t), cmp_seq_t);

        /* collect duplicates */
        /* allocate at most half the entries as duplicates */
        size_t dup_cap = all_len / 2 + 10;
        dup_vals = (seq_t *)malloc(dup_cap * sizeof(seq_t));
        if (!dup_vals) {
            free(all_vals);
            all_vals = NULL;
            return;
        }
        dup_len = 0;
        for (size_t i = 0; i < all_len; ) {
            size_t j = i + 1;
            while (j < all_len && all_vals[j] == all_vals[i])
                ++j;
            if (j - i >= 2) {
                if (dup_len == dup_cap) {
                    dup_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(dup_vals, dup_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(dup_vals);
                        dup_vals = NULL;
                        free(all_vals);
                        all_vals = NULL;
                        return;
                    }
                    dup_vals = tmp;
                }
                dup_vals[dup_len++] = all_vals[i];
            }
            i = j;
        }

        free(all_vals);
        all_vals = NULL;

        if (dup_len >= needed)
            break;                      /* satisfied */
        /* otherwise enlarge the bound */
        free(dup_vals);
        dup_vals = NULL;
        dup_len = 0;
        S_MAX *= 2;
    }
#endif
}

/* The requested sequence function ------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset is 1, but caller may check */
    build_cache(n);
#ifdef __cplusplus
    if ((idx_t)dup_vals.size() < n)
        return 0;
    return dup_vals[n - 1];
#else
    if (dup_len < n)
        return 0;
    return dup_vals[n - 1];
#endif
}
