/*
 * A273037  Discriminator of the first n Fermat numbers A000215.
 * https://oeis.org/A273037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer k such that the first n
 *            Fermat numbers F_i = 2^(2^i)+1 (i = 0 … n-1) are pairwise
 *            incongruent modulo k
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether the first m Fermat numbers are distinct modulo k.
   For k == 1 this is true only when m == 1. */
static bool check_distinct(seq_t k, idx_t m)
{
    if (k == (seq_t)1) return m == 1;

    size_t sz = (size_t)k;          /* assume k fits in size_t */
    static char *seen = NULL;
    static size_t seen_sz = 0;

    if (sz > seen_sz) {              /* grow buffer if necessary */
        free(seen);
        seen = (char *)malloc(sz);
        if (!seen) return false;    /* treat allocation fail as not distinct */
        seen_sz = sz;
    }
    memset(seen, 0, sz);

    seq_t val = (seq_t)2 % k;        /* 2^(2^0) mod k */
    for (idx_t i = 0; i < m; ++i) {
        seq_t r = (val + (seq_t)1) % k;      /* F_i mod k */
        size_t idx = (size_t)r;              /* assume r fits in size_t */
        if (seen[idx]) return false;
        seen[idx] = 1;
        val = (val * val) % k;               /* next exponent: square */
    }
    return true;
}

/* Main function requested by the OEIS kernel. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based cache of already computed values */
    static idx_t cache_len = 0;   /* current maximum index stored */

    if (n == 0) return 0; /* undefined, but safe guard */

    /* Grow cache if necessary */
    if (n > cache_len) {
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)(n + 1));
        if (!tmp) return 0;           /* allocation failure */
        cache = tmp;
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) cache[i] = 0;

        /* Compute missing values, starting from previous maximum. */
        for (idx_t m = cache_len + 1; m <= n; ++m) {
            seq_t start_k = (m == 1) ? (seq_t)1 : cache[m - 1];
            seq_t k = start_k;

            /* Search forward until distinctness holds. */
            while (true) {
                if (check_distinct(k, m)) {
                    cache[m] = k;
                    break;
                }
                ++k;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
