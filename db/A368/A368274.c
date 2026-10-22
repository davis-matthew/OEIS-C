/*
 * A368274  a(0) = 0, a(1) = 1, for n >= 2, a(n) = a(n-1) - 6*floor(median(a))
 *          - 9*floor(mean(a)). Median(a) and mean(a) are respectively the
 *          median and mean of all previous terms.
 * https://oeis.org/A368274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1 For n ≥ 2: a(n) = a(n‑1) - 6 * floor( median(
 *            a[0] … a[n‑2] ) ) - 9 * floor( mean( a[0] … a[n‑2] ) ) Median is
 *            the median of the first n‑1 terms (the previous ones). Mean is
 *            the floor of the average of the previous terms. The function
 *            memoises all computed terms, so calls may a...
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   floor_div_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_div_seq.h"

/* ---------- local helpers ---------------------------------- */

/* Ensure the capacity of a buffer.  New capacity is at least new_len
   and is doubled each time for amortised O(1) growth.           */
static void ensure_capacity(seq_t **buf, size_t *cap, size_t new_len)
{
    if (*cap < new_len) {
        size_t new_cap = (*cap == 0) ? 2 : (*cap << 1);
        if (new_cap < new_len)
            new_cap = new_len;
        *buf = (seq_t *)realloc(*buf, new_cap * sizeof(seq_t));
        if (*buf == NULL)
            exit(EXIT_FAILURE); /* allocation error */
        *cap = new_cap;
    }
}

/* Insert a value into an array sorted in ascending order.
   The array is assumed to have capacity at least sorted_size+1. */
static void insert_sorted(seq_t *sorted, size_t *sorted_size, seq_t val)
{
    size_t i, pos = 0;
    /* Find insertion point. */
    for (i = 0; i < *sorted_size; ++i) {
        if (!(sorted[i] < val)) {
            pos = i;
            break;
        }
    }
    if (i == *sorted_size) /* value is largest */
        pos = *sorted_size;

    /* Shift elements to the right. */
    for (i = *sorted_size; i > pos; --i)
        sorted[i] = sorted[i - 1];

    sorted[pos] = val;
    (*sorted_size)++;
}

/* ---------- memoised sequence implementation ---------------- */

/* Static cache state */
static seq_t *cache       = NULL;   /* computed terms      */
static size_t cache_cap   = 0;      /* capacity           */
static size_t cache_size  = 0;      /* number of terms    */

static seq_t *sorted       = NULL;   /* sorted array       */
static size_t sorted_cap   = 0;      /* capacity           */
static size_t sorted_size  = 0;      /* number of sorted   */

static seq_t sum = 0; /* running sum of all computed terms */

/* The required function */
seq_t a(idx_t n)
{
    size_t sn = (size_t)n;   /* for array indexing (n is ≤ 10⁴ in tests) */

    /* Initialise base terms on the first call. */
    if (cache_size == 0) {
        ensure_capacity(&cache, &cache_cap, 2);
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
        sum = cache[0] + cache[1];

        ensure_capacity(&sorted, &sorted_cap, 2);
        sorted[0] = 0;
        sorted[1] = 1;
        sorted_size = 2;
    }

    /* Return immediately if already computed. */
    if (sn < cache_size)
        return cache[sn];

    /* Compute terms up to sn inclusive. */
    for (size_t j = cache_size; j <= sn; ++j) {
        size_t len = j; /* number of previous terms */

        /* Median of the first len terms (the sorted array). */
        seq_t median;
        if (len & 1ULL) { /* odd */
            median = sorted[len >> 1];
        } else {          /* even */
            seq_t low  = sorted[(len >> 1) - 1];
            seq_t high = sorted[(len >> 1)];
            median = floor_div_seq(low + high, 2);
        }

        /* Mean of the first len terms. */
        seq_t mean = floor_div_seq(sum, (idx_t)len);

        /* New term according to the recurrence. */
        seq_t term = cache[j - 1] - (seq_t)6 * median - (seq_t)9 * mean;

        /* Grow cache if needed. */
        ensure_capacity(&cache, &cache_cap, j + 1);
        cache[j] = term;

        /* Insert new term into the sorted array. */
        ensure_capacity(&sorted, &sorted_cap, sorted_size + 1);
        insert_sorted(sorted, &sorted_size, term);

        /* Update the running sum. */
        sum += term;
    }

    cache_size = sn + 1;
    return cache[sn];
}
