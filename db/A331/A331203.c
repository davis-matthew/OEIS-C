/*
 * A331203  Numbers k such that k/(digsum(k)) is an integer cube.
 * https://oeis.org/A331203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k/DIGSUM(k) is an integer cube.
   This implementation searches k in increasing order and
   stores all found terms in a static array.
   The result is memoised, so subsequent calls are fast
   even if the index is not monotonic. */
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_cube_root.h"

static seq_t *terms = NULL;          /* accumulated terms */
static size_t capacity = 0;          /* size of allocated array */
static size_t length  = 0;           /* number of terms stored */
static seq_t next_k = 0;             /* next candidate to test */

// Grow capacity to at least needed slots
static void ensure_capacity(size_t needed)
{
    if (capacity < needed) {
        size_t new_cap = capacity == 0 ? 8 : capacity * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();            // cannot recover
        terms = tmp;
        capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Return n-th term (1‑based). */
    if (n == 0) return 0;                         // undefined, but safe

    /* Grow the term list until we have at least n elements. */
    while (length < (size_t)n) {
        next_k++;                                  // candidate k

        seq_t dsum = digit_sum(next_k);
        if (next_k % dsum != 0) continue;          // not divisible

        seq_t q = next_k / dsum;                   // cube candidate
        seq_t r = floor_cube_root(q);              // integer cube root

        if (r * r * r == q) {                     // perfect cube?
            ensure_capacity(length + 1);
            terms[length++] = next_k;
        }
    }
    return terms[(size_t)n - 1];
}
