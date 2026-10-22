/*
 * A374725  The "multiplicative comma sequence": the lexicographically
 *          earliest sequence of positive integers with the property that the
 *          sequence formed by the pairs of digits adjacent to the commas
 *          between the terms is the same as the sequence of successive ratios
 *          between the terms.
 * https://oeis.org/A374725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   pow10int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374725 – multiplicative comma sequence
 *
 *   a(1) = 1
 *   a(n) for n > 1 is defined as follows:
 *      let x be the least significant digit of a(n‑1);
 *      find the smallest y ∈ {1,…,9} such that
 *          a(n) = a(n‑1) * (10·x + y)
 *          has most‑significant digit y.
 *      If such y exists, a(n) is that value; otherwise the sequence ends.
 *
 * The implementation below memoises the sequence in a static cache.
 * a(idx_t n) returns a(n) for n ≥ 1, or 0 if the requested index is
 * beyond the termination of the sequence.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow10int.h"

/* Helper: number of decimal digits of a value. */
static int digit_len(seq_t v)
{
    if (v == 0) return 1;
    int len = 0;
    while (v > 0) {
        ++len;
        v /= (seq_t)10;
    }
    return len;
}

/* Static memoisation cache: cache[0] holds a(1). */
static seq_t *cache = NULL;   /* cache[0] = 1 for a(1) */
static idx_t cache_cap = 0;   /* number of filled elements (0‑based) */

/* Ensure that cache contains values up to index 'up_to' (0‑based). */
static void ensure_cache(idx_t up_to)
{
    if (up_to < cache_cap) return;          /* already computed */

    /* Grow cache to store indices up to `up_to`. */
    idx_t new_cap = up_to + 1;               /* elements: 0 … up_to */
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;

    /* Initialise first element if required. */
    if (cache_cap == 0) {
        cache[0] = (seq_t)1;    /* a(1) */
        cache_cap = 1;
    }

    /* Compute further terms until requested index or sequence ends. */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        seq_t prev = cache[i-1];
        seq_t x = prev % (seq_t)10;             /* least significant digit */

        bool found = false;
        seq_t candidate = 0;

        for (int y = 1; y <= 9; ++y) {
            seq_t m = (seq_t)10 * x + (seq_t)y;
            seq_t cand = prev * m;

            int d = digit_len(cand);
            seq_t msd = cand / pow10int(d-1);

            if (msd == (seq_t)y) {
                candidate = cand;
                found = true;
                break;  /* smallest y chosen */
            }
        }

        if (!found) {
            /* Sequence terminates before reaching index i. */
            cache_cap = i;   /* valid elements are < i */
            return;
        }

        cache[i] = candidate;
    }

    /* All requested indices were computed. */
    cache_cap = new_cap;
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for non‑positive indices */

    /* We use 0‑based cache indices: cache[0] ↔ a(1). */
    ensure_cache(n-1);

    if (n-1 < cache_cap)
        return cache[n-1];
    /* Requested index lies beyond the sequence termination. */
    return (seq_t)0;
}
