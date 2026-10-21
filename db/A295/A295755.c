/*
 * A295755  Solution of the complementary equation a(n) = a(n-1) + a(n-3) +
 *          a(n-4) + b(n-3), where a(0) = 1, a(1) = 2, a(2) = 3, a(3) = 4,
 *          b(0) = 5, b(1) = 6, b(2) = 7, b(3) = 8, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295755 (complementary sequences) implementation.
   Recurrence: \
   a(0)=1, a(1)=2, a(2)=3, a(3)=4, \
   b(0)=5, b(1)=6, b(2)=7, b(3)=8, \
   a(n)=a(n-1)+a(n-3)+a(n-4)+b(n-3)   (n≥4)   */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *A = NULL;           /* array for a(n)   */
static seq_t *B = NULL;           /* array for b(n)   */
static size_t cap = 0;            /* current allocation capacity   */
static idx_t  maxComputed = (idx_t)-1; /* highest n that has been computed   */
static seq_t  last_b      = 0;    /* last b(n) value   */

/* Binary search for val in A[0..upto] (inclusive). */
static bool a_contains(idx_t upto, seq_t val) {
    idx_t lo = 0, hi = upto;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (A[mid] == val) return true;
        if (A[mid] < val)  lo = mid + 1;
        else               hi = mid - 1;
    }
    return false;
}

/* Increase capacity to hold at least needed+1 elements. */
static void ensure_capacity(idx_t needed) {
    if ((size_t)needed + 1 <= cap) return;
    size_t new_cap = cap ? cap : 4;
    while (new_cap <= (size_t)needed + 1) new_cap <<= 1;
    A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    B = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Public function required by the specification. */
seq_t a(idx_t n) {
    /* Lazy initialization of the base case. */
    if (A == NULL) {
        cap = 4;
        A = (seq_t *)malloc(cap * sizeof(seq_t));
        B = (seq_t *)malloc(cap * sizeof(seq_t));
        A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 4;
        B[0] = 5; B[1] = 6; B[2] = 7; B[3] = 8;
        last_b   = 8;
        maxComputed = 3;
    }

    if (n <= maxComputed) return A[n];

    ensure_capacity(n);

    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t val = A[i-1] + A[i-3] + A[i-4] + B[i-3];
        A[i] = val;

        /* Find the next b(i) as the smallest integer not in a[0..i-1]. */
        seq_t cand = last_b + 1;
        while (a_contains(i, cand)) ++cand;
        B[i] = cand;
        last_b = cand;
    }

    maxComputed = n;
    return A[n];
}
