/*
 * A293357  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + n +1, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A293357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2) + n + 1 a(0) = 1, a(1) = 3 b(0)
 *            = 2, b(1) = 4 where b(k) is the k‑th positive integer not
 *            appearing in the sequence a() (the complementary sequence)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent static state for memoisation */
    static seq_t *A = NULL;              /* a values */
    static seq_t *B = NULL;              /* b values (complement) */
    static idx_t sizeA = 0;              /* allocated size of A */
    static idx_t sizeB = 0;              /* allocated size of B */
    static idx_t max_n = 0;              /* highest a index computed */
    static seq_t nextCandidate = 1;      /* next number to test for b */
    static idx_t next_b_index = 0;       /* current length of B */
    static idx_t a_check_idx = 0;        /* helper index for membership check */

    /* Initialise on first call */
    if (!A) {
        sizeA = 8;  A = (seq_t*)malloc(sizeA * sizeof(seq_t));
        sizeB = 8;  B = (seq_t*)malloc(sizeB * sizeof(seq_t));

        /* base values */
        A[0] = (seq_t)1;
        A[1] = (seq_t)3;
        max_n = 1;

        /* generate the first two missing numbers (b[0], b[1]) */
        nextCandidate = 1;
        a_check_idx = 0;
        next_b_index = 0;

        while (next_b_index < 2) {
            /* advance a_check_idx to the first a >= nextCandidate */
            while (a_check_idx <= max_n && A[a_check_idx] < nextCandidate)
                a_check_idx++;
            if (a_check_idx <= max_n && A[a_check_idx] == nextCandidate) {
                /* candidate is used by a, skip it */
                nextCandidate++;
                continue;
            }
            /* candidate is free, record into b */
            B[next_b_index++] = (seq_t)nextCandidate;
            nextCandidate++;
        }
    }

    /* Return cached value if available */
    if (n <= max_n)
        return A[n];

    /* Compute a(n) for all indices up to n, expanding arrays as needed */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Ensure b array has entries up to i-2 for recurrence. */
        while (next_b_index <= i - 2) {
            /* For each new b value, find the next integer not used by a. */
            /* Advance to the first a that is >= nextCandidate. */
            while (a_check_idx <= max_n && A[a_check_idx] < nextCandidate)
                a_check_idx++;
            if (a_check_idx <= max_n && A[a_check_idx] == nextCandidate) {
                /* The candidate is in a; skip it. */
                nextCandidate++;
                continue;
            }

            /* Grow B as necessary */
            if (next_b_index >= sizeB) {
                sizeB <<= 1;
                B = (seq_t*)realloc(B, sizeB * sizeof(seq_t));
            }
            B[next_b_index++] = (seq_t)nextCandidate;
            nextCandidate++;
        }

        /* Grow A as necessary */
        if (i >= sizeA) {
            sizeA <<= 1;
            A = (seq_t*)realloc(A, sizeA * sizeof(seq_t));
        }

        /* Compute a[i] using the recurrence */
        A[i] = A[i - 1] + A[i - 2] + B[i - 2] + (seq_t)i + (seq_t)1;
    }

    max_n = n;
    return A[n];
}
