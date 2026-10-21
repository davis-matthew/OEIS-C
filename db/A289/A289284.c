/*
 * A289284  Starting with a(1) = 1, a(n) = smallest nonnegative integer not
 *          yet in the sequence such that the last digit of a(n-1) plus the
 *          first digit of a(n) is equal to 6. The digit 0 is not allowed.
 * https://oeis.org/A289284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   used_exists
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A289284 – computed iteratively by scanning candidates.
   a(1) = 1.
   For n > 1, a(n) is the smallest non‑negative integer not already
   in the sequence such that
        (last digit of a(n‑1)) + (first digit of a(n)) = 6,
   and neither a(n‑1) nor a(n) may contain the digit 0.
   The sequence terminates after 31 terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/used_exists.h"

/* Helper: returns true if the number contains the digit 0.
   Works for built‑in unsigned types and for arbitrary‑precision
   types that support % and / with integer operands. */
static bool has_zero_digit(seq_t v)
{
    if (v == 0) return true;
    while (v > 0) {
        uint8_t d = (uint8_t)(unsigned long long)(v % 10);
        if (d == 0) return true;
        v /= 10;
    }
    return false;
}

/* Helper: returns the first (most significant) digit of v. */
static int first_digit(seq_t v)
{
    while (v >= 10) v /= 10;
    return (int)(unsigned long long)v;
}

/* Helper: returns the last (least significant) digit of v. */
static inline int last_digit(seq_t v)
{
    return (int)(unsigned long long)(v % 10);
}

/* Helper: grow a dynamic array to at least min_cap elements. */
static void grow_array(seq_t **arr, size_t *cap, size_t min_cap)
{
    if (*cap >= min_cap) return;
    while (*cap < min_cap) *cap = (*cap == 0) ? 8 : (*cap << 1);
    *arr = (seq_t *)realloc(*arr, (*cap) * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1 */

    static seq_t *seq   = NULL;   /* term array */
    static size_t seq_len = 0, seq_cap = 0;

    static seq_t *used   = NULL;  /* set of used numbers */
    static size_t used_len = 0, used_cap = 0;

    /* Initialise with the first term. */
    if (seq_len == 0) {
        grow_array(&seq, &seq_cap, 8);
        seq[0] = 1;
        seq_len = 1;

        grow_array(&used, &used_cap, 8);
        used[0] = 1;
        used_len = 1;
    }

    /* Extend the cache until it reaches the requested index. */
    for (; seq_len < n; ++seq_len) {
        seq_t prev = seq[seq_len - 1];
        int last = last_digit(prev);
        int first_needed = 6 - last;

        /* If the required first digit cannot be 1–9, the sequence ends. */
        if (first_needed < 1 || first_needed > 9)
            return 0;

        /* Scan for the smallest unused candidate that satisfies the
           first‑digit constraint and contains no zero. */
        seq_t candidate = 1;
        for (;;) {
            if (used_exists(used, used_len, candidate))   candidate++;
            else if (has_zero_digit(candidate))          candidate++;
            else if (first_digit(candidate) != first_needed) candidate++;
            else break;    /* candidate is valid */
        }

        /* Append the new term. */
        grow_array(&seq, &seq_cap, seq_len + 1);
        seq[seq_len] = candidate;

        grow_array(&used, &used_cap, used_len + 1);
        used[used_len++] = candidate;
    }

    return seq[n - 1];
}
