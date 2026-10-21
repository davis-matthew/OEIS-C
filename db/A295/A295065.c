/*
 * A295065  Solution of the complementary equation a(n) = 8*a(n-3) + b(n-2),
 *          where a(0) = 1, a(1) = 3, a(2) = 5, b(0) = 2, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-3) + b(n-2), a(0)=1, a(1)=3, a(2)=5, b(0)=2. b is
 *            the complementary increasing sequence to a
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Auxiliary function to grow a seq_t array when needed */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t newcap = (*cap == 0) ? 1 : *cap;
    while (newcap < need)
        newcap <<= 1;          /* double the size until enough */
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Static storage for the sequences and helpers */
static seq_t *a_seq   = NULL, *b_seq   = NULL;     /* arrays of values */
static size_t a_cap = 0, b_cap = 0;                /* capacities */
static idx_t  a_len = 0, b_len = 0;                /* current lengths */

/* For searching the next unused integer we keep a single forward
   candidate and two indices into the already known a[] and b[] values
   (both individually sorted). */
static seq_t candidate = 1;        /* next integer to test */
static idx_t  scan_a   = 0;        /* next a[scan_a] >= candidate, if any */
static idx_t  scan_b   = 0;        /* next b[scan_b] >= candidate, if any */

/* Find the next positive integer that is not already in a[] or b[] */
static seq_t get_next_unused(void)
{
    for (;;) {
        /* Skip a values that are smaller than the candidate */
        while (scan_a < a_len && a_seq[scan_a] < candidate)
            ++scan_a;
        if (scan_a < a_len && a_seq[scan_a] == candidate) {
            ++candidate;
            continue;
        }
        /* Skip b values that are smaller than the candidate */
        while (scan_b < b_len && b_seq[scan_b] < candidate)
            ++scan_b;
        if (scan_b < b_len && b_seq[scan_b] == candidate) {
            ++candidate;
            continue;
        }
        /* Found an unused integer */
        seq_t res = candidate;
        ++candidate;
        return res;
    }
}

/* Ensure that the sequences are computed up to index n */
static void ensure_up_to(idx_t n)
{
    if (a_len == 0) {
        /* Initialise base values for a[] */
        grow_seq_array(&a_seq, &a_cap, 3);
        a_seq[0] = (seq_t)1;
        a_seq[1] = (seq_t)3;
        a_seq[2] = (seq_t)5;
        a_len   = 3;

        /* Compute first three values of b[] via complement */
        grow_seq_array(&b_seq, &b_cap, 3);
        for (idx_t i = 0; i < 3; ++i) {
            b_seq[i] = get_next_unused();
            ++b_len;
        }
    }

    /* Compute further values until we have a[n] */
    while (a_len <= n) {
        /* a[i] = 8*a[i-3] + b[i-2]  (i == current a_len) */
        seq_t val = (seq_t)8 * a_seq[a_len - 3] + b_seq[a_len - 2];

        grow_seq_array(&a_seq, &a_cap, a_len + 1);
        a_seq[a_len] = val;
        ++a_len;

        /* Compute the corresponding b value */
        grow_seq_array(&b_seq, &b_cap, b_len + 1);
        b_seq[b_len] = get_next_unused();
        ++b_len;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_seq[n];
}
