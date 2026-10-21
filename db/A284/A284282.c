/*
 * A284282  a(n) = the number k such that A030067(2k-1) = n, or 0 if n does
 *          not occur in the semi-Fibonacci sequence A030067.
 * https://oeis.org/A284282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A030067(2k-1) = n, otherwise 0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A030067 is defined by:  a(1)=1;
   for n even  : a(n) = a(n/2);
   for n odd   : a(n) = a(n-1) + a(n-2).                     */
#include <stdlib.h>
#include <stdint.h>

/* Static storage for the generated values of A030067 */
static seq_t *a_vals = NULL;      /* 1‑based indexing, a_vals[0] unused */
static size_t a_cap = 0;
static size_t a_len = 0;          /* number of entries stored (highest index) */

/* Static storage for the strictly increasing subsequence of odd indices */
static seq_t *odd_vals = NULL;    /* odd_vals[0] = first odd value (k=1) */
static size_t odd_cap = 0;
static size_t odd_len = 0;

/* Ensure that a_vals contains values up to max_index (1‑based). */
static void ensure_a_vals(size_t max_index) {
    if (a_len >= max_index) return;

    /* Initialise if first time */
    if (a_len == 0) {
        a_cap = 16;
        a_vals = (seq_t*)calloc(a_cap, sizeof(seq_t));
        if (!a_vals) exit(EXIT_FAILURE);
        a_vals[1] = (seq_t)1;
        a_vals[2] = a_vals[1];
        a_len   = 2;
    }

    /* Grow capacity if necessary */
    if (max_index >= a_cap) {
        while (a_cap <= max_index) a_cap <<= 1;
        a_vals = (seq_t*)realloc(a_vals, a_cap * sizeof(seq_t));
        if (!a_vals) exit(EXIT_FAILURE);
    }

    /* Compute new terms */
    for (size_t i = a_len + 1; i <= max_index; ++i) {
        if ((i & 1) == 0)                      /* even index */
            a_vals[i] = a_vals[i / 2];
        else {                                 /* odd index */
            if (i == 1)                      /* base case */
                a_vals[i] = (seq_t)1;
            else
                a_vals[i] = a_vals[i - 1] + a_vals[i - 2];
        }
    }
    a_len = max_index;
}

/* Extend odd_vals until its last member is at least target. */
static void extend_odd_vals(idx_t target) {
    while (odd_len == 0 || odd_vals[odd_len - 1] < (seq_t)target) {
        idx_t next_k = (idx_t)odd_len + 1;          /* k = next position in odd_vals */
        idx_t idx    = 2 * next_k - 1;             /* the odd index in A030067 */

        ensure_a_vals((size_t)idx);                /* make sure a_vals[idx] exists */

        /* Grow odd_vals array if needed */
        if (odd_len == odd_cap) {
            odd_cap = odd_cap ? odd_cap << 1 : 16;
            odd_vals = (seq_t*)realloc(odd_vals, odd_cap * sizeof(seq_t));
            if (!odd_vals) exit(EXIT_FAILURE);
        }
        odd_vals[odd_len++] = a_vals[idx];
    }
}

/* Main function required by the skeleton. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    extend_odd_vals(n);                    /* generate odd_vals up to n */

    /* Binary search on odd_vals. */
    size_t left  = 0;
    size_t right = odd_len;                /* exclusive upper bound */

    seq_t target = (seq_t)n;
    while (left < right) {
        size_t mid = left + ((right - left) >> 1);
        if (odd_vals[mid] < target)
            left = mid + 1;
        else
            right = mid;
    }

    if (left < odd_len && odd_vals[left] == target)
        return (seq_t)(left + 1);           /* k = position + 1 */
    return (seq_t)0;
}
