/*
 * A295068  Solution of the complementary equation a(n) = 2*a(n-2) - b(n-1) +
 *          n, where a(0) = 4, a(1) = 5, b(0) = 1, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4, a(1)=5, b(0)=1; a(n) = 2*a(n-2) - b(n-1) + n */
#include <stddef.h>
#include <stdlib.h>

/* Resize array to at least min_cap elements */
static void ensure_cap(seq_t **arr, size_t *cap, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap ? *cap * 2 : 4);
        while (new_cap < min_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Memoization state */
static seq_t *a_arr = NULL, *b_arr = NULL;
static size_t a_cap = 0, b_cap = 0;
static size_t a_size = 0, b_size = 0;

/* State used for generating the complementary sequence b */
static idx_t next_candidate = 0;          /* next integer to test for membership in b */
static size_t a_ptr_b = 0;                /* index into a_arr for membership test */

/* Generate the next value of b using the current a_arr */
static seq_t next_b_value(void) {
    while (1) {
        /* Advance a_ptr_b to skip a values smaller than candidate */
        while (a_ptr_b < a_size && a_arr[a_ptr_b] < next_candidate)
            ++a_ptr_b;
        if (a_ptr_b < a_size && a_arr[a_ptr_b] == next_candidate) {
            /* candidate is in a; skip it */
            ++next_candidate;
            continue;
        }
        /* candidate is not in a; it becomes the next b value */
        seq_t val = (seq_t)next_candidate;
        ++next_candidate;
        return val;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure initialization on first call */
    if (!a_arr) {
        /* a(0) and a(1) */
        ensure_cap(&a_arr, &a_cap, 2);
        a_arr[0] = (seq_t)4;
        a_arr[1] = (seq_t)5;
        a_size = 2;
        /* b(0) */
        ensure_cap(&b_arr, &b_cap, 1);
        b_arr[0] = (seq_t)1;
        b_size = 1;
        next_candidate = 2;
        a_ptr_b = 0;
    }

    if (n == 0) return a_arr[0];
    if (n == 1) return a_arr[1];

    /* Compute terms up to requested n */
    for (idx_t i = (idx_t)a_size; i <= n; ++i) {
        /* Ensure b(i-1) exists */
        if (b_size <= (size_t)(i-1)) {
            ensure_cap(&b_arr, &b_cap, b_size + 1);
            b_arr[b_size++] = next_b_value();
        }

        /* Recurrence: a(i) = 2*a(i-2) - b(i-1) + i */
        seq_t term = (seq_t)2 * a_arr[i-2] - b_arr[i-1] + (seq_t)i;

        ensure_cap(&a_arr, &a_cap, a_size + 1);
        a_arr[a_size++] = term;
    }

    return a_arr[n];
}
