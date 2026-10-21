/*
 * A277145  Lexicographically least sequence of nonnegative integers that
 *          avoids 7/4-powers.
 * https://oeis.org/A277145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9334, u16 n <= 9334, u32 n <= 9334, u64 n <= 9334,
 *            u128 n <= 9334, big any n
 * Helpers:   causes_7_4_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: lexicographically least sequence of nonnegative integers
   avoiding forbidden 7/4‑powers.  The function builds the sequence
   incrementally, memoising the results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/causes_7_4_power.h"

/* Static storage for the sequence, shared across calls */
static seq_t  *seq_cache    = NULL;
static size_t seq_len      = 0;     /* current number of stored terms */
static size_t seq_capacity = 0;     /* allocated capacity */

seq_t a(idx_t n)
{
    /* Initialise the cache lazily */
    if (seq_len == 0) {
        seq_capacity = 1024;
        seq_cache    = (seq_t*)malloc(seq_capacity * sizeof(seq_t));
        seq_cache[0] = 0;      /* a(0) = 0 */
        seq_len      = 1;
    }

    /* Grow the cache if needed */
    while ((size_t)n + 1 > seq_capacity) {
        seq_capacity *= 2;
        seq_cache = (seq_t*)realloc(seq_cache, seq_capacity * sizeof(seq_t));
    }

    /* Compute the sequence up to index n */
    if ((size_t)n + 1 > seq_len) {
        for (size_t pos = seq_len; pos <= (size_t)n; ++pos) {
            seq_t val = 0;
            for (;;) {
                if (!causes_7_4_power(seq_cache, pos, val)) {
                    seq_cache[pos] = val;   /* accept the lexicographically
                                               smallest legal value */
                    break;
                }
                ++val;  /* try the next integer */
            }
        }
        seq_len = (size_t)n + 1;
    }

    return seq_cache[(size_t)n];
}
