/*
 * A351009  Numbers k such that the k-th composition in standard order is a
 *          concatenation of distinct twins (x,x).
 * https://oeis.org/A351009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A351009 by searching positive integers k and testing
   whether the composition of k (in standard graded reverse‑lexicographic
   order) is a concatenation of distinct twin parts (x,x).  The found k
   values are cached in a static array so that repeated calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t *idx_cache = NULL;   /* cached valid k's */
static size_t idx_cap   = 0;
static size_t idx_len   = 0;
static idx_t  next_k    = 0;       /* next integer to test */

/* Append a new value to the cache, growing the array when needed.  */
static void append_idx(idx_t val)
{
    if (idx_len == idx_cap) {
        size_t new_cap = (idx_cap == 0) ? 16 : idx_cap * 2;
        idx_t *tmp = (idx_t *)realloc(idx_cache, new_cap * sizeof(idx_t));
        if (!tmp) abort();          /* out of memory */
        idx_cache = tmp;
        idx_cap   = new_cap;
    }
    idx_cache[idx_len++] = val;
}

/* Test whether a given k produces a composition that is a concatenation
   of distinct twin parts (x,x).  */
static bool is_valid(idx_t k)
{
    /* Extract positions of 1 bits (1‑indexed).  */
    idx_t  pos[64];
    size_t m = 0;
    idx_t  temp = k;
    idx_t  bit_pos = 1;
    while (temp) {
        if (temp & 1ULL) pos[m++] = bit_pos;
        temp >>= 1;
        bit_pos++;
    }

    /* Empty composition (k=0) counts as valid.  */
    if (m == 0) return true;

    /* The number of parts (m) must be even.  */
    if (m & 1) return false;

    /* Compute differences between consecutive positions.  */
    idx_t diff[64];
    diff[0] = pos[0];
    for (size_t i = 1; i < m; i++) diff[i] = pos[i] - pos[i-1];

    /* Check twin property on the reversed difference array.  */
    /* Reversed order: diff[m-1], diff[m-2], ..., diff[0]  */
    idx_t twin_vals[32];
    size_t twin_cnt = 0;

    for (size_t j = 0; j + 1 < m; j += 2) {
        idx_t part1 = diff[m-1-j];
        idx_t part2 = diff[m-2-j];
        if (part1 != part2) return false;  /* not a twin */

        /* Ensure distinctness among twins.  */
        for (size_t t = 0; t < twin_cnt; ++t)
            if (twin_vals[t] == part1) return false;
        twin_vals[twin_cnt++] = part1;
    }
    return true;
}

/* Return the n‑th term of A351009 (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* unused offset */
    /* Generate values until we have at least n cached terms.  */
    while (idx_len < n) {
        if (is_valid(next_k))
            append_idx(next_k);
        next_k++;
    }
    return (seq_t)idx_cache[n - 1];
}
