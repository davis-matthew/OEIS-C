/*
 * A374797  Distinct value of A003990 (LCM table read by antidiagonals) in
 *          order of appearance.
 * https://oeis.org/A374797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A374797: distinct values of A003990 (LCM table read by antidiagonals)
   in the order they first appear. The sequence is a permutation of the positive integers.
   The function keeps a static cache so that calls can be made in any order. */

#include <stdlib.h>
#include "../utils/gcd.h"      /* provides: seq_t gcd(seq_t a, seq_t b); */

/* Ensure that an array can hold at least 'need' elements. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Return the n-th term of A374797 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* Undefined for n < 1; return 0 as a sentinel */

    /* Static cache and state variables */
    static seq_t *cache = NULL;      /* array of distinct LCM values seen so far */
    static size_t cap   = 0;         /* current capacity of the cache */
    static size_t len   = 0;         /* number of stored distinct values */

    static idx_t cur_k = 1;          /* current anti‑diagonal index (i + j - 1) */
    static idx_t cur_i = 1;          /* current i within that anti‑diagonal */

    /* Generate values until we have at least n distinct entries */
    while (len < n) {
        /* Coordinates (i, j) for the current entry */
        idx_t j = cur_k - cur_i + 1;            /* j >= 1, since cur_i <= cur_k */
        seq_t ii = (seq_t)cur_i;
        seq_t jj = (seq_t)j;

        /* Compute LCM(i, j) = i / gcd(i, j) * j */
        seq_t g = gcd(ii, jj);
        seq_t lcm_val = (ii / g) * jj;

        /* Check if lcm_val is already in the cache (linear scan) */
        int seen = 0;
        for (size_t p = 0; p < len; ++p) {
            if (cache[p] == lcm_val) { seen = 1; break; }
        }

        /* Store new distinct value */
        if (!seen) {
            ensure_cap(&cache, &cap, len + 1);
            cache[len++] = lcm_val;
        }

        /* Move to the next entry in the anti‑diagonal traversal */
        ++cur_i;
        if (cur_i > cur_k) {
            ++cur_k;
            cur_i = 1;
        }
    }

    return cache[n - 1];  /* 1‑based indexing */
}
