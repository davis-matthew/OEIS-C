/*
 * A392708  Numbers k such that the 0-based partial alternating sums of the
 *          k-th composition in standard order are all distinct.
 * https://oeis.org/A392708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 3905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A392708: numbers k such that the 0‑based partial alternating
   sums of the k‑th composition in standard order are all distinct.
   Implementation enumerates k in increasing order starting from 0,
   tests the distinctness condition, and memoises the qualifying k
   values.  As the number of parts of a composition equals the
   popcount of k (≤ 64 for 64‑bit k), all intermediate arrays are
   bounded.  The 0‑based partial alternating sums are computed with
   the recurrence
          s_0 = 0
          s_j = -s_{j-1} + p_j   (p_j : parts of the composition)
   which requires only the parts in reverse order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MAX_PARTS
#define MAX_PARTS 128   /* safe upper bound for number of parts */
#endif

/* Check whether the 0‑based partial alternating sums of the composition
   corresponding to k are all distinct. */
static bool qualifies(idx_t k)
{
    /* Extract positions of 1‑bits (1‑based) while computing differences.
       The differences are stored in the order they appear; they will be
       processed in reverse to obtain the composition parts. */
    seq_t diffs[MAX_PARTS];
    unsigned m = 0;          /* number of parts */
    int last_pos = 0;        /* last position (1‑based), starts with 0 */
    int pos = 1;             /* current position (1‑based) */
    idx_t mask = k;

    while (mask) {
        if (mask & (idx_t)1) {
            if (m >= MAX_PARTS) return false; /* overflow guard */
            diffs[m++] = (seq_t)(pos - last_pos);
            last_pos = pos;
        }
        mask >>= 1;
        ++pos;
    }

    /* Compute the partial alternating sums while checking for duplicates.
       We keep an array of seen sums (including s_0 = 0). */
    seq_t seen[MAX_PARTS + 1];
    unsigned seen_cnt = 0;
    seq_t sum = (seq_t)0;          /* s_0 */
    seen[seen_cnt++] = sum;

    /* Process parts in reverse order (i.e. composition parts). */
    for (long i = (long)m - 1; i >= 0; --i) {
        sum = -sum + diffs[i];      /* s_j = -s_{j-1} + p_j           */
        /* Check against all previously seen sums. */
        for (unsigned j = 0; j < seen_cnt; ++j) {
            if (seen[j] == sum) return false;
        }
        seen[seen_cnt++] = sum;
    }
    return true;
}

/* Memoised generation of the sequence.
   cache[i] holds the i‑th qualifying k (0‑based indexing). */
static seq_t *cache = NULL;
static size_t cached_len = 0;
static size_t cached_cap = 0;
static idx_t next_k_to_test = 0;

static void ensure_cache(size_t n)
{
    if (cached_len >= n) return;

    if (cached_cap == 0) {
        cached_cap = 64;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache) abort();
    } else if (cached_len == cached_cap) {
        cached_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    while (cached_len < n) {
        if (qualifies(next_k_to_test)) {
            cache[cached_len++] = (seq_t)next_k_to_test;
        }
        ++next_k_to_test;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety for non‑positive indices */
    /* a(1) is the first term, so shift to 0‑based index. */
    n -= 1;
    ensure_cache(n + 1);
    return cache[n];
}
