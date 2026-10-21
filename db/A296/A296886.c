/*
 * A296886  Numbers whose base-11 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pits_gt_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A296886.
   The function searches natural numbers, converts each to base‑11,
   counts pits and peaks, and stores those with more pits than peaks.
   a(1) = 122, a(2) = 123, … as in the sequence definition. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pits_gt_peaks.h"

/* maximum number of elements we expect to need in the tests */
#define MAX_CACHE_SIZE 20000

static seq_t cache[MAX_CACHE_SIZE];
static size_t cache_len = 0;          /* number of terms currently stored */
static seq_t next_candidate = (seq_t)1;  /* next natural number to check */

/* Fill cache with terms until it has at least n elements. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;  /* already have enough */

    uint8_t digits[128];
    size_t dlen;

    while (cache_len < (size_t)n) {
        if (!pits_gt_peaks(next_candidate, digits, &dlen)) {
            ++next_candidate;
            continue;
        }

        /* store the term */
        cache[cache_len++] = next_candidate;

        /* advance to next candidate */
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
