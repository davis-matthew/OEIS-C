/*
 * A248756  a(n) = smallest k such that a(n-k) and n have the same number of
 *          1's in their binary expansions, or a(n) = n if no such k exists.
 * https://oeis.org/A248756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear, look
 * Formula:   a(n) = smallest k>0 such that popcount(a(n-k)) == popcount(n).
 *            If no such k exists, a(n) = n. (1-indexed, a(1)=1)
 * Range:     u8 n <= 364, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* popcount for seq_t values; works for any integral width or arbitrary‑precision class */
static inline unsigned int popcount_seq(seq_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= (x - 1);  /* clear lowest set bit */
        ++c;
    }
    return c;
}

/* static cache for computed a(n) values */
static seq_t *cache = NULL;       /* array of a(1)..a(n) */
static idx_t  cache_len = 0;      /* number of elements currently stored (max n computed) */
static idx_t  cache_cap = 0;      /* capacity of cache */

/* last occurrence of an a(j) having a given popcount (1..64) */
static idx_t last_index[65] = {0};

seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed; caller will handle */
        cache_len = 0;
        memset(last_index, 0, sizeof(last_index));
    }

    /* grow cache if necessary */
    if (n > cache_cap) {
        idx_t newcap = cache_cap ? cache_cap * 2 : 64;
        while (n > newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing values up to n */
    for (idx_t j = cache_len + 1; j <= n; ++j) {
        unsigned int pc_j = popcount_seq(j);   /* popcount of j */
        idx_t     prev   = last_index[pc_j];    /* last index with same popcount */

        seq_t val;
        if (prev == 0) {
            /* no previous match */
            val = (seq_t)j;
        } else {
            val = (seq_t)(j - prev);           /* smallest k = j - prev */
        }

        cache[j - 1] = val;                       /* store a(j) */

        /* update last occurrence for a(j)'s popcount */
        unsigned int pc_val = popcount_seq(val);
        last_index[pc_val] = j;
    }

    cache_len = n;
    return cache[n - 1];
}
