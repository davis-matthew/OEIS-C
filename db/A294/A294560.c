/*
 * A294560  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          2*b(n-1) + 2*b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294560
   a(0) = 1, a(1) = 2
   a(n) = a(n-1) + a(n-2) + 2*b(n-1) + 2*b(n-2)
   (a) and (b) are complementary increasing sequences beginning with
   b(0) = 3.
   b(n) is defined as the least natural number not already used in
   a(0..n-1) or b(0..n-1).
   The function stores values in static caches so calls can be in any
   order.  Complexity is O(n log n) with n the largest argument seen. */

#include <stdlib.h>
#include <stdbool.h>

/* static caches */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t a_cap = 0;
static size_t b_cap = 0;
static idx_t a_len = 0;   /* number of a terms stored */
static idx_t b_len = 0;   /* number of b terms stored */
static idx_t next_candidate = 1;   /* next integer to test when building b */

/* binary search: return 1 if val occurs in a[0..len-1] */
static bool a_contains(seq_t val, idx_t len) {
    idx_t lo = 0, hi = len - 1;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t mid_val = a_cache[mid];
        if (mid_val == val)
            return true;
        if (mid_val < val)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

/* Ensure capacity of a_cache or b_cache */
static void ensure_capacity(size_t needed, size_t *cap,
                            void **ptr, size_t elem_size) {
    if (needed <= *cap)
        return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* main function */
seq_t a(idx_t n) {
    /* First time: initialise a(0) and a(1) */
    if (a_len == 0) {
        ensure_capacity(2, &a_cap, (void **)&a_cache, sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_len = 2;

        ensure_capacity(2, &b_cap, (void **)&b_cache, sizeof(seq_t));
        b_len = 0;
        next_candidate = 1;
    }

    /* Grow arrays to hold up to n */
    ensure_capacity((size_t)n + 1, &a_cap, (void **)&a_cache, sizeof(seq_t));
    ensure_capacity((size_t)n + 1, &b_cap, (void **)&b_cache, sizeof(seq_t));

    /* Compute terms up to n */
    for (idx_t k = a_len; k <= n; ++k) {
        /* Ensure b has indices up to k-1 before computing a[k] */
        while (b_len <= k - 1) {
            while (a_contains((seq_t)next_candidate, a_len))
                ++next_candidate;
            b_cache[b_len] = (seq_t)next_candidate;
            ++next_candidate;
            ++b_len;
        }

        /* Compute a[k] using already known b[k-1] and b[k-2] */
        seq_t val = a_cache[k - 1] + a_cache[k - 2]
                    + (seq_t)2 * b_cache[k - 1] + (seq_t)2 * b_cache[k - 2];
        a_cache[k] = val;
        ++a_len;
    }

    /* Store one more b (index n) for possible future use */
    while (b_len <= n) {
        while (a_contains((seq_t)next_candidate, a_len))
            ++next_candidate;
        b_cache[b_len] = (seq_t)next_candidate;
        ++next_candidate;
        ++b_len;
    }

    return a_cache[n];
}
