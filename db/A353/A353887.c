/*
 * A353887  Squarefree numbers of the form k^2 + k + 1 for some k >= 0.
 * https://oeis.org/A353887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353887: squarefree numbers of the form k^2 + k + 1 (k >= 0)
   computed by searching through successive k and testing squarefreeness.
   The function keeps a static cache so that any order of calls is fast. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Static cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;          /* number of terms stored in cache */
    static idx_t next_k = 0;       /* next k to test */

    /* Return cached term if already computed */
    if (n >= 1 && n <= len)
        return cache[n - 1];

    /* Ensure cache has some capacity */
    if (cap == 0) {
        cap = 1024;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we reach the nth */
    while (len < n) {
        /* Compute k^2 + k + 1 as a seq_t */
        seq_t k_seq = (seq_t)next_k;
        seq_t val = k_seq * k_seq + k_seq + 1;

        if (is_squarefree(val)) {
            /* Store the term, growing the cache if necessary */
            if ((size_t)len >= cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = val;
        }

        next_k++;
    }

    return cache[n - 1];
}
