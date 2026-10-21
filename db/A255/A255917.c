/*
 * A255917  Triangular numbers n such that each decimal digit of n is equal to
 *          the difference of at least two other digits of n.
 * https://oeis.org/A255917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_tri_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by incremental search for triangular numbers satisfying the
 * digit-difference property described in OEIS A255917. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_tri_valid.h"

/* Returns the n-th term of A255917 (1‑based index). */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t processed_k = 0;          /* largest k processed so far */

    if (n == 0) return (seq_t)0;           /* undefined for offset 0 */

    while (cache_len < n) {
        ++processed_k;
        seq_t kseq = (seq_t)processed_k;
        seq_t tri = kseq * (kseq + 1) / 2;
        if (is_tri_valid(tri)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {  /* allocation failure, abort */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = tri;
        }
    }
    return cache[n - 1];
}
