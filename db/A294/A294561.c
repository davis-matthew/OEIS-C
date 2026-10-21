/*
 * A294561  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          2*b(n-1) + b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294561
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

/* Recurrence: a(n) = a(n-1) + a(n-2) + 2*b(n-1) + b(n-2)
 * Initial values: a(0)=1, a(1)=2, b(0)=3, b(1)=4
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* allocate sequences up to n */
    size_t len = (size_t)(n + 1);
    seq_t *A = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *B = (seq_t*)malloc(len * sizeof(seq_t));
    if (!A || !B) abort();       /* memory unavailable */

    /* initial values */
    A[0] = (seq_t)1;  B[0] = (seq_t)3;
    A[1] = (seq_t)2;  B[1] = (seq_t)4;

    seq_t next_candidate = (seq_t)5;  /* next unused integer for b */
    idx_t a_skip = 2;               /* first index of A that may be >= next_candidate */

    for (idx_t i = 2; i <= n; ++i) {
        /* compute a(i) from recurrence */
        A[i] = A[i-1] + A[i-2] + (seq_t)2 * B[i-1] + B[i-2];

        /* assign b(i) as the next unused integer */
        B[i] = next_candidate;
        /* advance candidate for the next b value */
        next_candidate++;

        /* skip over any a values that equal next_candidate */
        while (a_skip <= i && A[a_skip] < next_candidate)
            a_skip++;

        while (a_skip <= i && A[a_skip] == next_candidate) {
            next_candidate++;
            a_skip++;
        }
    }

    seq_t result = A[n];
    free(A);
    free(B);
    return result;
}
