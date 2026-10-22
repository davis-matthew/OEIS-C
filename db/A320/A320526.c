/*
 * A320526  a(n) is the number of chiral pairs of color patterns (set
 *          partitions) in a row of length n using exactly 3 colors (subsets).
 * https://oeis.org/A320526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (S2(n,3) - A(n,3)) / 2 with S2(n,3) = (3^n - 3·2^n + 3)/6
 *            and A(n,3) defined by the recurrence A(n,k) = [n>1]·(k·A(n-2,k)
 *            + A(n-2,k-1) + A(n-2,k-2)) + [n<2 & n==k & n>=0] . Initial
 *            values: a(1)=0, a(2)=0, a(3)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Allocate arrays for A0..A3 up to index n. */
    size_t len = (size_t)n + 1;
    seq_t *A0 = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *A1 = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *A2 = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *A3 = (seq_t *)calloc(len, sizeof(seq_t));
    if (!A0 || !A1 || !A2 || !A3)
    {
        free(A0); free(A1); free(A2); free(A3);
        return (seq_t)0;
    }

    /* Base values */
    A0[0] = (seq_t)1;
    if (len > 1)
    {
        A1[1] = (seq_t)1;   /* A(1,1) */
    }

    /* Build A(n,1), A(n,2), A(n,3) for 2 ≤ n ≤ given n */
    for (size_t i = 2; i <= n; ++i)
    {
        /* A(n,1) = A(n-2,1) + A(n-2,0) */
        A1[i] = A1[i - 2] + A0[i - 2];

        /* A(n,2) = 2·A(n-2,2) + A(n-2,1) + A(n-2,0) */
        A2[i] = (seq_t)2 * A2[i - 2] + A1[i - 2] + A0[i - 2];

        /* A(n,3) = 3·A(n-2,3) + A(n-2,2) + A(n-2,1) */
        A3[i] = (seq_t)3 * A3[i - 2] + A2[i - 2] + A1[i - 2];
    }

    /* Stirling number of the second kind S2(n,3) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t S2   = pow3 - (seq_t)3 * pow2 + (seq_t)3;
    S2 = S2 / (seq_t)6;

    seq_t result = (S2 - A3[n]) / (seq_t)2;

    /* Free temporary arrays */
    free(A0); free(A1); free(A2); free(A3);

    return result;
}
