/*
 * A294418  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + 2*b(n-2), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A294418
    Complementary sequences a(n) = a(n-1)+a(n-2)+b(n-1)+2*b(n-2)
    with a(0)=1, a(1)=3, b(0)=2, b(1)=4.
    a(0) is the first term listed. */

/*  This implementation uses memoisation.  It grows two dynamic arrays
    A[ ] and B[ ] to store required terms.  The helper function
    next_b_value() finds the next natural number that is not already
    present in the a‑sequence.  All arithmetic is done in seq_t, which
    may be an arbitrary‑precision integer class in the C++ build.  The
    code compiles under both C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>

/* static arrays and state for memoisation */
static seq_t *A = NULL;      /* a-values   */
static size_t capA = 0;      /* capacity for A   */
static idx_t lenA = 0;       /* number of a-values stored */

static seq_t *B = NULL;      /* b-values   */
static size_t capB = 0;      /* capacity for B   */
static idx_t lenB = 0;       /* number of b-values stored */

static seq_t next_candidate = (seq_t)2;   /* next natural to try for b */
static idx_t skip_idx = 0;                /* index into A when skipping */

// --------------------------------------------------------------------
// ensure_cap: make sure the dynamic array has enough capacity
static void ensure_cap(size_t *cap, seq_t **arr, idx_t needed)
{
    if (*cap < (size_t)needed) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap * 2);
        while (new_cap < (size_t)needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

// --------------------------------------------------------------------
// next_b_value: find the next integer not present in the a‑sequence
static seq_t next_b_value(void)
{
    for (;;) {
        /* skip over any a-values that are smaller than the candidate */
        while (skip_idx < lenA && A[skip_idx] < next_candidate)
            ++skip_idx;

        /* if the candidate equals an a-value, skip it */
        if (skip_idx < lenA && A[skip_idx] == next_candidate) {
            ++next_candidate;
            ++skip_idx;
            continue;
        }

        /* otherwise the candidate is a valid b-value */
        seq_t v = next_candidate;
        ++next_candidate;
        return v;
    }
}

// --------------------------------------------------------------------
// a: compute the n-th term of A294418
seq_t a(idx_t n)
{
    static bool initialized = false;

    if (!initialized) {
        /* initial allocation */
        capA = 4;
        A = (seq_t *)malloc(capA * sizeof(seq_t));
        capB = 4;
        B = (seq_t *)malloc(capB * sizeof(seq_t));

        /* base cases */
        A[0] = (seq_t)1;
        A[1] = (seq_t)3;
        lenA = 2;

        B[0] = next_b_value();   /* 2 */
        B[1] = next_b_value();   /* 4 */
        lenB = 2;

        initialized = true;
    }

    /* return stored value if already computed */
    if (n < lenA)
        return A[n];

    /* compute until we reach n */
    for (idx_t k = lenA; k <= n; ++k) {
        /* ensure arrays have enough space */
        ensure_cap(&capA, &A, k + 1);
        ensure_cap(&capB, &B, k + 1);

        /* recurrence for a(k) */
        seq_t val = A[k - 1] + A[k - 2] + B[k - 1] + ((seq_t)2) * B[k - 2];
        A[k] = val;
        ++lenA;

        /* next complementary term */
        B[k] = next_b_value();
        ++lenB;
    }

    return A[n];
}
