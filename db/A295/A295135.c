/*
 * A295135  Solution of the complementary equation a(n) = 3*a(n-1) + b(n-1) -
 *          2, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + b(n-1) - 2, with a(0) = 1, a(1) = 2, b(0) = 3.
 *            The numbers b(n) are the smallest unused integers so that the
 *            increasing sequences a() and b() are complementary
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Allocate an array to hold the generated a values. */
    seq_t *A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!A) return (seq_t)0;                /* allocation failure → 0 */

    A[0] = (seq_t)1;
    A[1] = (seq_t)2;

    seq_t next_b = (seq_t)4;      /* the smallest unused integer after a[1] */
    idx_t ptr = 0;                /* index into A for skipping used values */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Skip any earlier a-values that equal or are less than next_b. */
        while (ptr < i && A[ptr] <= next_b)
        {
            if (A[ptr] == next_b)   /* this number cannot be used as b */
                next_b++;
            ptr++;
        }

        seq_t b_val = next_b;      /* current b(n-1) value */
        next_b++;                  /* advance to the next candidate integer */

        A[i] = 3 * A[i - 1] + b_val - (seq_t)2;   /* recurrence for a(n) */
    }

    seq_t result = A[n];
    free(A);
    return result;
}
