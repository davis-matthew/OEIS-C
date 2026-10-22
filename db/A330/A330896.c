/*
 * A330896  Lexicographically earliest sequence of positive integers such that
 *          for any m > 0, gaps between consecutive m's are all distinct.
 * https://oeis.org/A330896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Lexicographically earliest integer sequence where for each m>0 the
 *  gaps between consecutive occurrences of m are all distinct.
 *  This is OEIS A330896.  The implementation uses a greedy construction
 *  and memoises all terms up to the largest n requested, so that calls
 *  in arbitrary order remain O(current max n).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Per‑value bookkeeping */
struct value_info {
    idx_t last_pos;     /* last position this value appeared (0 if none) */
    size_t gaps_cnt;    /* number of distinct gaps stored */
    size_t gaps_cap;    /* allocated capacity for gaps array */
    idx_t *gaps;        /* dynamic array of used gaps */
};

/* Ensure the global array of value_info is large enough. */
static void ensure_values_capacity(size_t needed,
                                   struct value_info **arr,
                                   size_t *cap)
{
    if (needed < *cap) return;
    size_t new_cap = (*cap == 0 ? 4 : (*cap << 1));
    while (new_cap <= needed) new_cap <<= 1;
    *arr = (struct value_info *)realloc(*arr,
                                         new_cap * sizeof(struct value_info));
    for (size_t i = *cap; i < new_cap; ++i) {
        (*arr)[i].last_pos = 0;
        (*arr)[i].gaps_cnt = 0;
        (*arr)[i].gaps_cap = 0;
        (*arr)[i].gaps     = NULL;
    }
    *cap = new_cap;
}

/* Append a new gap to the list for a value, growing the array if needed. */
static void append_gap(struct value_info *info, idx_t g)
{
    if (info->gaps_cnt == info->gaps_cap) {
        size_t new_cap = (info->gaps_cap == 0 ? 4 : (info->gaps_cap << 1));
        info->gaps = (idx_t *)realloc(info->gaps,
                                      new_cap * sizeof(idx_t));
        info->gaps_cap = new_cap;
    }
    info->gaps[info->gaps_cnt++] = g;
}

/* Ensure the sequence array has room for 'needed' terms. */
static void ensure_seq_capacity(size_t needed,
                                seq_t **seq_arr,
                                size_t *cap)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0 ? 4 : (*cap << 1));
    while (new_cap <= needed) new_cap <<= 1;
    *seq_arr = (seq_t *)realloc(*seq_arr,
                                new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static struct value_info *values = NULL;
    static size_t values_cap   = 0;
    static seq_t *seq = NULL;
    static size_t seq_len   = 0;   /* number of terms already computed */
    static size_t seq_cap   = 0;   /* current allocation size */

    if (n == 0) { /* offset is 1, but guard just in case */
        return 0;
    }

    /* If already computed, just return. */
    if ((size_t)n <= seq_len) {
        return seq[(size_t)n - 1];
    }

    /* Grow sequence array to accommodate the new terms. */
    ensure_seq_capacity((size_t)n, &seq, &seq_cap);

    /* Compute any missing terms up to n. */
    for (idx_t pos = seq_len + 1; pos <= n; ++pos) {
        /* Find the smallest integer v that can occur at position pos. */
        for (idx_t v = 1; ; ++v) {
            ensure_values_capacity((size_t)v, &values, &values_cap);
            struct value_info *info = &values[(size_t)v];

            if (info->last_pos == 0) {            /* first occurrence of v */
                info->last_pos = pos;
                seq[(size_t)pos - 1] = (seq_t)v;
                break;
            }

            idx_t gap = pos - info->last_pos;     /* new gap for v */
            bool used = false;
            for (size_t i = 0; i < info->gaps_cnt; ++i) {
                if (info->gaps[i] == gap) { used = true; break; }
            }
            if (!used) {                          /* v can be used here */
                append_gap(info, gap);
                info->last_pos = pos;
                seq[(size_t)pos - 1] = (seq_t)v;
                break;
            }
            /* otherwise try the next candidate value v+1 */
        }
    }
    seq_len = (size_t)n;
    return seq[(size_t)n - 1];
}
