/*
 * A294419  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          2*b(n-1) + 2*b(n-2), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*b(n-1) + 2*b(n-2); a(0) = 1, a(1) =
 *            3; b(0) = 2, b(1) = 4
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   used_in_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/used_in_array.h"

/* Ensure the dynamic array has room for at least 'need' elements. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need) {
    if (*cap >= need)
        return;
    size_t new_cap = *cap ? *cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;  /* double until big enough */
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Static storage for the sequences and state. */
static seq_t *A = NULL;          /* a(n) values */
static seq_t *B = NULL;          /* b(n) values */
static size_t A_cap = 0, B_cap = 0;
static seq_t next_candidate = (seq_t)3;   /* smallest unused integer after init */
static idx_t computed_n   = (idx_t)-1;    /* highest index computed */

/* Compute all terms up to and including n. */
static void compute_up_to(idx_t n) {
    /* Initialize on first call. */
    if (computed_n == (idx_t)-1) {
        /* Array size for index 0. */
        ensure_cap(&A, &A_cap, 1);
        ensure_cap(&B, &B_cap, 1);
        A[0] = (seq_t)1;
        B[0] = (seq_t)2;
        computed_n = 0;
        /* next_candidate already set to 3. */
    }

    if (computed_n >= n)
        return;  /* already computed */

    /* Grow arrays to hold up to n. */
    ensure_cap(&A, &A_cap, (size_t)n + 1);
    ensure_cap(&B, &B_cap, (size_t)n + 1);

    for (idx_t i = computed_n + 1; i <= n; ++i) {
        seq_t ai;
        if (i == 1)
            ai = (seq_t)3;
        else
            ai = A[i - 1] + A[i - 2] + (seq_t)2 * B[i - 1] + (seq_t)2 * B[i - 2];
        A[i] = ai;

        /* Find b(i): smallest unused positive integer. */
        seq_t cand = next_candidate;
        while (used_in_array(A, (size_t)i + 1, cand) ||
               used_in_array(B, (size_t)i,     cand))
            ++cand;
        B[i] = cand;

        /* Advance next_candidate for next iteration. */
        next_candidate = cand + 1;
        while (used_in_array(A, (size_t)i + 1, next_candidate) ||
               used_in_array(B, (size_t)i + 1, next_candidate))
            ++next_candidate;
    }

    computed_n = n;
}

/* Public interface. */
seq_t a(idx_t n) {
    compute_up_to(n);
    return A[n];
}
