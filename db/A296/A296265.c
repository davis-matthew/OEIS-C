/*
 * A296265  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1), where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1) a(0) = 3, a(1) = 4 b(0) =
 *            1, b(1) = 2 a(n) and b(n) are increasing complementary sequences
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *A = NULL;           /* array of a values   */
static seq_t *B = NULL;           /* array of b values   */
static size_t A_len = 0;          /* number of elements computed */
static size_t cap = 0;            /* current capacity of arrays */

/* Grow the arrays to at least new_cap elements */
static void grow_to(size_t new_cap) {
    seq_t *newA, *newB;
#if defined(__cplusplus)
    newA = new seq_t[new_cap];
    newB = new seq_t[new_cap];
    for (size_t i = 0; i < A_len; ++i) {
        newA[i] = A[i];
        newB[i] = B[i];
    }
    delete[] A;
    delete[] B;
#else
    newA = (seq_t*)malloc(new_cap * sizeof(seq_t));
    newB = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!newA || !newB) abort();
    memcpy(newA, A, A_len * sizeof(seq_t));
    memcpy(newB, B, A_len * sizeof(seq_t));
    free(A);
    free(B);
#endif
    A = newA;
    B = newB;
    cap = new_cap;
}

static void ensure_arrays(size_t need) {
    if (cap == 0) {
        size_t init = 4;
        if (need > init) init = need;
        grow_to(init);
        /* initialise first two terms */
        A[0] = (seq_t)3;
        A[1] = (seq_t)4;
        B[0] = (seq_t)1;
        B[1] = (seq_t)2;
        A_len = 2;
    } else if (cap < need) {
        size_t new_cap = cap * 2;
        if (new_cap < need) new_cap = need;
        grow_to(new_cap);
    }
}

static int a_in_a_array(seq_t value, size_t upto) {
    for (size_t i = 0; i < upto; ++i) {
        if (A[i] == value) return 1;
    }
    return 0;
}

seq_t a(idx_t n) {
    /* Handle initial terms without allocating arrays */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)4;

    /* Ensure arrays can hold at least n+1 elements */
    ensure_arrays(n + 1);

    /* Compute missing terms up to index n */
    while (A_len <= (size_t)n) {
        size_t i = A_len; /* index to compute */

        /* recurrence for a[i] */
        seq_t prev_a1 = A[i - 1];
        seq_t prev_a2 = A[i - 2];
        seq_t prev_b1 = B[i - 1];
        seq_t prev_b2 = B[i - 2];
        A[i] = prev_a1 + prev_a2 + prev_b1 * prev_b2;

        /* compute b[i] as smallest unused positive integer > b[i-1] */
        seq_t cand = B[i - 1] + (seq_t)1;
        while (a_in_a_array(cand, i)) { /* skip values that are in a[0..i-1] */
            cand = cand + (seq_t)1;
        }
        B[i] = cand;

        ++A_len; /* A_len and B_len stay equal */
    }

    return A[n];
}
