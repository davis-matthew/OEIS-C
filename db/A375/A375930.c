/*
 * A375930  Numbers k such that A005117(k+1) - A005117(k) > 1. In other words,
 *          the k-th squarefree number is more than 1 less than the next.
 * https://oeis.org/A375930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the index k such that A005117(k+1) – A005117(k) > 1
   (i.e., successive square‑free numbers differ by more than 1).  The
   routine builds the terms by a linear scan using the helper
   `is_squarefree`. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/grow_seq.h"

/* Static cache of the sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* State of the running scan */
static idx_t  processed_upto = 0;   /* largest integer examined so far */
static seq_t  prev_sf        = 0;   /* previous square‑free number */
static idx_t  prev_k         = 0;   /* its index */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, just return 0 */

    /* Grow the cache until it contains at least n elements. */
    if (cache_size < n) {
        for (idx_t num = processed_upto + 1; ; ++num) {

            if (is_squarefree((seq_t)num)) {
                idx_t k = prev_k + 1;                /* current index */

                if (prev_sf > 0) {
                    seq_t diff = (seq_t)num - prev_sf;
                    if (diff > 1) {                  /* a hit */
                        if (cache_size == cache_cap)
                            grow_seq(&cache, &cache_cap, cache_size + 1);
                        cache[cache_size++] = (seq_t)prev_k; /* store k */
                    }
                }

                prev_sf = (seq_t)num;
                prev_k  = k;
            }

            processed_upto = num;        /* advance the global pointer */

            if (cache_size >= n) break;   /* enough terms obtained */
        }
    }

    return cache[n - 1];
}
