/*
 * A295067  Solution of the complementary equation a(n) = 2*a(n-2) + b(n-2),
 *          where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 5, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + b(n-2); * Initial values: a(0)=1, a(1)=3;
 *            b(0)=2, b(1)=5; * a and b are increasing complementary sequences
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Allocate arrays for a[] and b[] */
    size_t len = (size_t)n + 1;
    seq_t *A = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *B = (seq_t*)malloc(len * sizeof(seq_t));

    /* Allocate a boolean array marking which integers have been used.
       The smallest unused integer never exceeds 2*n+3, so a size of
       2*(n+2)+10 is sufficient. */
    size_t cap = 2 * ((size_t)n + 2) + 10;
    bool *used = (bool*)calloc(cap + 1, sizeof(bool));

    /* Initialize known elements */
    A[0] = (seq_t)1;  A[1] = (seq_t)3;
    B[0] = (seq_t)2;  B[1] = (seq_t)5;

    /* Mark the initial used numbers */
    if (1 <= cap) used[1] = true;
    if (2 <= cap) used[2] = true;
    if (3 <= cap) used[3] = true;
    if (5 <= cap) used[5] = true;

    /* Helper to mark a number if it is within the array bounds. */
    #define MARK_USED(v) \
        do { if ((v) <= cap) used[(size_t)(v)] = true; } while (0)

    MARK_USED(A[0]); MARK_USED(A[1]); MARK_USED(B[0]); MARK_USED(B[1]);

    /* Candidate for the next b[i] : start at 1 and advance as numbers
       are used. */
    size_t cand = 1;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute a[i] from the recurrence. */
        A[(size_t)i] = (seq_t)2 * A[(size_t)(i - 2)] + B[(size_t)(i - 2)];
        MARK_USED(A[(size_t)i]);

        /* Find the smallest unused integer for b[i] */
        while (cand < cap && used[cand]) cand++;
        B[(size_t)i] = (seq_t)cand;
        used[cand] = true;
        cand++; /* next search starts from the following integer */
    }

    seq_t result = A[(size_t)n];

    /* Clean up */
    free(used);
    free(A);
    free(B);
    #undef MARK_USED

    return result;
}
