/*
 * A333379  Numbers k such that the k-th composition in standard order is
 *          weakly increasing and covers an initial interval of positive
 *          integers.
 * https://oeis.org/A333379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer k such that the k-th composition
   in standard order is weakly increasing and covers an initial
   interval of positive integers. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: check whether k qualifies the required condition. */
static bool qualifies(idx_t k)
{
    if (k == 0)
        return true;  /* empty composition is accepted */

    idx_t t = k;
    int diff[64];
    int cnt = 0;
    idx_t prev_pos = 0;
    int prev_diff = -1;  /* to check nonincreasing */

    /* collect the differences of successive set‑bit positions */
    for (idx_t pos = 1; t != 0; ++pos) {
        if (t & 1) {
            int d = (int)(pos - prev_pos);
            diff[cnt++] = d;
            if (cnt > 1 && d > prev_diff) /* not nonincreasing */
                return false;
            prev_diff = d;
            prev_pos = pos;
        }
        t >>= 1;
    }

    int max_val = diff[0];          /* first difference is the largest */
    bool seen[65] = {false};        /* max possible max_val <= 64 */

    for (int i = 0; i < cnt; ++i)
        seen[diff[i]] = true;

    for (int v = 1; v <= max_val; ++v)
        if (!seen[v])
            return false;

    return true;
}

/* Cache to store previously found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of cache array */
static idx_t next_k = 0;       /* next integer to test */

/* Main function: returns the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    /* silently ignore n == 0 (invalid since OFFSET = 1) */
    while (cache_len < (size_t)n) {
        if (qualifies(next_k)) {
            /* ensure capacity */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
