/*
 * A345910  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum -1.
 * https://oeis.org/A345910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   alt_sum_is_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345910: numbers k such that the alternating sum of the k‑th
   composition (in standard order) equals -1.
   Computed by brute‑force enumeration of k and rejection testing. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/alt_sum_is_minus_one.h"

/* Maximum number of terms we can cache.  The sequence grows
   reasonably slowly, so 200 000 covers far more than the test
   ranges. */
#define CACHE_CAPACITY 200000

static seq_t cache[CACHE_CAPACITY];
static size_t cache_len = 0;
static idx_t cur_k = 0;  /* last k value tested */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Build up the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        ++cur_k;                            /* next k to test         */

        if (alt_sum_is_minus_one(cur_k)) {
            if (cache_len == CACHE_CAPACITY) {
                /* In the unlikely event the cache overflows, stop
                   generating; the requested n exceeded the limit. */
                break;
            }
            cache[cache_len++] = (seq_t)cur_k;
        }
    }

    /* Guard against request beyond pre‑allocated capacity.  For
       genuine uses n will always be within bounds. */
    if ((size_t)n > cache_len)
        return 0;   /* indicates error / out of range */

    return cache[(size_t)n - 1];
}
