/*
 * A296280  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1)*b(n), where a(0) = 1, a(1) = 4, b(0) = 2, b(1) = 3,
 *          b(2) = 5, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 81, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A296280
   Recurrence: a(0)=1, a(1)=4,
               a(n)=a(n-1)+a(n-2)+b(n-2)*b(n-1)*b(n)  (n>=2)
   Complementary sequences: (a(n)) and (b(n)) are increasing,
   together covering all positive integers.
   We compute a(n) by generating both sequences simultaneously.
   The algorithm is O(n) and uses only simple integer arithmetic. */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the n-th term of sequence A296280. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Allocate arrays for a[0..n] and b[0..n+2] (a few extra slots). */
    size_t szA = (size_t)n + 1;
    size_t szB = (size_t)n + 3;           /* enough for indices up to n */
    seq_t *A = (seq_t *)calloc(szA, sizeof(seq_t));
    seq_t *B = (seq_t *)calloc(szB, sizeof(seq_t));
    if (!A || !B) {
        free(A);
        free(B);
        return (seq_t)0;                  /* allocation failure (unlikely) */
    }

    /* Initial conditions */
    A[0] = (seq_t)1;
    A[1] = (seq_t)4;
    B[0] = (seq_t)2;
    B[1] = (seq_t)3;
    B[2] = (seq_t)5;

    idx_t cur      = 6;            /* next integer candidate for b */
    idx_t ai_idx   = 0;            /* index into A for scanning a's */
    idx_t lastAIdx = 1;            /* largest a index that has been computed */

    /* Main loop: compute a(n) for n>=2, and generate the next b value each time. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) = a(i-1) + a(i-2) + b(i-2)*b(i-1)*b(i) */
        seq_t prod = B[i-2];
        prod *= B[i-1];
        prod *= B[i];
        A[i] = A[i-1] + A[i-2] + prod;

        /* Update the largest a index used so far. */
        lastAIdx = i;

        /* Generate b(i+1) for the next iteration (if needed). */
        /* We only need b indices up to n+1; after final iteration this step is skipped. */
        if (i + 1 <= n + 2) {
            /* Advance 'cur' until it is not equal to any known a value. */
            for (;;) {
                /* Skip a values that are ≤ cur. */
                while (ai_idx <= lastAIdx && cur > A[ai_idx]) ai_idx++;

                /* If cur equals a known a value, skip this integer. */
                if (ai_idx <= lastAIdx && cur == A[ai_idx]) {
                    ai_idx++;
                    cur++;
                    continue;          /* try next integer */
                }

                /* cur is not an a value -> it is the next b. */
                B[i+1] = (seq_t)cur;
                cur++;                /* advance for future searches */
                break;
            }
        }
    }

    seq_t result = A[n];
    free(A);
    free(B);
    return result;
}
