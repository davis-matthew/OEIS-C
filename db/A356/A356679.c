/*
 * A356679  The lexicographically earliest infinite squarefree sequence of
 *          nonnegative integers that starts with 3.
 * https://oeis.org/A356679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The lexicographically earliest infinite squarefree word of
 *            nonnegative integers starting with 3, built greedily
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for the sequence values. */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but kept safe */

    /* Initialise the cache with the first term. */
    if (cache_len == 0) {
        cache_len = 1;
        cache_cap = 1;
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
        seq_cache[0] = 3;   /* a(1) = 3 */
    }

    /* Extend the cache up to index n. */
    while (cache_len < n) {
        size_t len = cache_len;            /* current length of sequence */
        seq_t best = 0;                    /* candidate value */

        /* Find the smallest non‑negative integer that keeps the word
           squarefree after appending it. */
        for (seq_t v = 0;; ++v) {
            int square = 0;                /* flag whether a square appears */

            size_t kmax = (len + 1) / 2;   /* maximum block size to test */
            for (size_t k = 1; k <= kmax && !square; ++k) {
                size_t pos = len - k;      /* first element of the second block */

                if (seq_cache[pos] != v) continue;   /* cannot form a square */

                /* A square would occur if the remaining k‑1 elements also match. */
                if (k == 1) {              /* two equal consecutive elements */
                    square = 1;
                    break;
                }

                size_t start1 = len - 2 * k + 1;   /* start of first block */
                size_t start2 = pos + 1;           /* start of second block */

                int ok = 1;
                for (size_t j = 0; j < k - 1; ++j) {
                    if (seq_cache[start1 + j] != seq_cache[start2 + j]) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    square = 1;
                    break;
                }
            }

            if (!square) {      /* v is valid */
                best = v;
                break;
            }
        }

        /* Grow the cache if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        seq_cache[cache_len] = best;
        ++cache_len;
    }

    return seq_cache[n - 1];
}
