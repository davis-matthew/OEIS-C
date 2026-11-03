/*
 * A247016  Triangular numbers A000217 composed of only curved digits {0, 2,
 *          3, 5, 6, 8, 9}.
 * https://oeis.org/A247016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 71, u32 n <= 2448, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9960 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of triangular numbers composed only of curved
   digits {0,2,3,5,6,8,9} by scanning triangular numbers incrementally. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>   /* used only in C mode realloc copy */

/* Append val to the dynamic cache array, expanding if necessary. */
static void add_to_cache(seq_t **cache, size_t *cap, size_t *len, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < *len; ++i)
            tmp[i] = (*cache)[i];
        delete[] *cache;
#else
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
#endif
        *cache = tmp;
        *cap   = new_cap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* Check if all decimal digits of x belong to the curved set. */
static bool is_curved(seq_t x)
{
    if (x == (seq_t)0) return true;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (!(d == (seq_t)0 || d == (seq_t)2 || d == (seq_t)3 ||
              d == (seq_t)5 || d == (seq_t)6 || d == (seq_t)8 ||
              d == (seq_t)9))
            return false;
        x /= (seq_t)10;
    }
    return true;
}

/* External linkage function requested by the test harness. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* safety guard, not required */

    /* static state – persists across calls in any order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_tri_n = 0;   /* triangular index (0,1,2,…) */

    size_t idx = (size_t)(n - (idx_t)1);   /* zero‑based cache index */

    while (cache_len <= idx) {
        /* compute the next triangular number safely in seq_t */
        seq_t tri = (seq_t)next_tri_n *
                    ((seq_t)next_tri_n + (seq_t)1) /
                    (seq_t)2;
        if (is_curved(tri))
            add_to_cache(&cache, &cache_cap, &cache_len, tri);
        ++next_tri_n;
    }
    return cache[idx];
}
