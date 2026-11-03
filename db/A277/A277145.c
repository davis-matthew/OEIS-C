/*
 * A277145  Lexicographically least sequence of nonnegative integers that
 *          avoids 7/4-powers.
 * https://oeis.org/A277145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 136 terms through n = 9334 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: lexicographically least sequence of nonnegative integers
   avoiding forbidden 7/4‑powers.  The function builds the sequence
   incrementally, memoising the results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether appending <cand> at position <pos> would create a
   7/4‑power.  The check examines all possible block lengths *k* such
   that 7*k* fits in the current (potential) sequence. */
static bool causes_7_4_power(const seq_t *seq, size_t pos, seq_t cand)
{
    /* Length after adding the candidate */
    size_t new_len = pos + 1;
    for (size_t k = 1; 7 * k <= new_len; ++k) {
        size_t start = new_len - 7 * k;          /* block start index */

        /* Quick rejection: the last element of the 3k‑block must equal
           the candidate, otherwise the pattern cannot match. */
        if (seq[start + 3 * k - 1] != cand)
            continue;

        /* Compare the first 3k elements of the block with the tail
           of the next 4k elements (subtracting the added candidate). */
        bool ok = true;
        for (size_t i = 0; i < 3 * k; ++i) {
            seq_t val = (i == 3 * k - 1) ? cand : seq[start + 4 * k + i];
            if (seq[start + i] != val) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;   /* would create a forbidden 7/4‑power */
    }
    return false;          /* candidate is safe */
}

/* Static storage for the sequence, shared across calls */
static seq_t  *seq_cache    = NULL;
static size_t seq_len      = 0;     /* current number of stored terms */
static size_t seq_capacity = 0;     /* allocated capacity */

seq_t a(idx_t n)
{
    /* Initialise the cache lazily */
    if (seq_len == 0) {
        seq_capacity = 1024;
        seq_cache    = mem_new(seq_t, seq_capacity);
        seq_cache[0] = 0;      /* a(0) = 0 */
        seq_len      = 1;
    }

    /* Grow the cache if needed */
    while ((size_t)n + 1 > seq_capacity) {
        seq_capacity *= 2;
        seq_cache = mem_resize(seq_t, seq_cache, seq_capacity);
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
