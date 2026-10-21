/*
 * A295964  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) - 1, where a(0) = 1, a(1) = 4, b(0) = 2, b(1) = 3, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 4 a(n) = a(n-1) + a(n-2) + b(n) – 1 (n ≥ 2) b(0)
 *            = 2, b(1) = 3, b(2) = 5 b(n) is the least positive integer not
 *            appearing in a(0..n-1) or in b(0..n-2); all values are
 *            increasing complementary sequences. The function below memoises
 *            the two sequences and returns any requested term a...
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   binary_search_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binary_search_seq.h"

seq_t a(idx_t n)
{
    /* static memoised tables */
    static seq_t *A = NULL;
    static seq_t *B = NULL;
    static size_t A_cap = 0;   /* allocated capacity */
    static size_t A_len = 0;   /* number of terms actually stored   */

    /* If we don't have enough space, grow it */
    if (n >= (idx_t)A_cap) {
        size_t new_cap = A_cap ? A_cap : 8;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmpA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        seq_t *tmpB = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        if (!tmpA || !tmpB) {   /* allocation failed – return 0 as failure mode */
            return (seq_t)0;
        }
        A = tmpA;
        B = tmpB;
        A_cap = new_cap;
    }

    /* Initialise base terms if this is the first call */
    if (A_len == 0) {
        A[0] = (seq_t)1;
        B[0] = (seq_t)2;
        if (A_cap > 1) {
            A[1] = (seq_t)4;
            B[1] = (seq_t)3;
        }
        if (A_cap > 2) {
            B[2] = (seq_t)5;
            A[2] = A[1] + A[0] + B[2] - (seq_t)1;  /* 9 */
        }
        A_len = (A_cap > 2) ? 3 : ((A_cap > 1) ? 2 : 1);
    }

    /* Generate terms up to the requested index n */
    for (size_t i = A_len; i <= (size_t)n; ++i) {
        /* find the next unused integer for b(i) */
        seq_t cand = B[i - 1] + (seq_t)1;  /* start from previous b(i-1)+1 */
        while (binary_search_seq(A, i, cand) ||          /* in a[0..i-1]     */
               binary_search_seq(B, i - 1, cand)) {       /* in b[0..i-2]     */
            ++cand;
        }
        B[i] = cand;

        /* recurrence for a(i) */
        if (i >= 2) {
            A[i] = A[i - 1] + A[i - 2] + B[i] - (seq_t)1;
        }
    }
    A_len = (size_t)n + 1;  /* we now have terms up to n */

    return A[(size_t)n];
}
