/*
 * A398950  Numbers k such that k, k+2, and k+4 are all squarefree.
 * https://oeis.org/A398950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A398950: numbers k such that k, k+2, and k+4 are all squarefree.
   Computed by testing odd k for squarefreeness of k, k+2 and k+4. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset */

    /* Memoized cache of the sequence terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_k = 1;     /* next odd integer to examine */

    while (cache_len < n) {
        seq_t k = (seq_t)next_k;
        if (is_squarefree(k) && is_squarefree(k + 2) && is_squarefree(k + 4)) {
            append_candidate(&cache, &cache_len, &cache_cap, k);
        }
        next_k += 2;             /* only odd candidates */
    }

    return cache[n - 1];
}
