/*
 * A320528  Number of chiral pairs of color patterns (set partitions) in a row
 *          of length n using exactly 5 colors (subsets).
 * https://oeis.org/A320528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (S2(n,5) - A(n,5)) / 2 where S2(n,k) satisfies S2(n,k) =
 *            k*S2(n-1,k) + S2(n-1,k-1) A(n,k) satisfies A(n,k) = k*A(n-2,k) +
 *            A(n-2,k-1) + A(n-2,k-2) (n>1) Base cases: S2(0,0)=1, S2(n,0)=0
 *            (n>0); A(0,0)=1, A(1,1)=1, otherwise 0. The function computes
 *            a(n) for n ≥ 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the usual integer types.  No other headers are required. */
#include <stdint.h>

/* The build supplies the types seq_t and idx_t. */

seq_t a(idx_t n)
{
    /* For n ≤ 5 the sequence is zero. */
    if (n <= 5)
        return (seq_t)0;

    /* Constants */
    const int K = 5;            /* number of colors */
    const int MAXK = 5;         /* largest k we need */

    /* Working arrays for the Stirling numbers S2(n,k) for k=0..5 */
    seq_t S2_prev[6] = {0};
    seq_t S2_curr[6] = {0};

    /* Working arrays for A(n,k) for k=0..5 */
    seq_t A_prev2[6] = {0}; /* row n-2   */
    seq_t A_prev1[6] = {0}; /* row n-1   */
    seq_t A_curr[6]  = {0}; /* row n     */

    /* --- Initialise n = 0 --- */
    S2_prev[0] = (seq_t)1;          /* S2(0,0)=1 */
    A_prev2[0] = (seq_t)1;          /* A(0,0)=1 */

    /* --- Initialise n = 1 --- */
    S2_curr[0] = (seq_t)0;
    S2_curr[1] = (seq_t)1;
    for (int k = 2; k <= MAXK; ++k)
        S2_curr[k] = (seq_t)0;

    /* After computing n=1, set S2_prev to row 1 */
    for (int k = 0; k <= MAXK; ++k)
        S2_prev[k] = S2_curr[k];

    A_prev1[0] = (seq_t)0;
    A_prev1[1] = (seq_t)1;
    for (int k = 2; k <= MAXK; ++k)
        A_prev1[k] = (seq_t)0;

    /* --- Main loop from n = 2 to desired n --- */
    seq_t result = 0; /* holds a(n) when i == n */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Stirling numbers */
        S2_curr[0] = (seq_t)0;
        for (int k = 1; k <= MAXK; ++k)
        {
            seq_t term1 = (seq_t)k * S2_prev[k];
            seq_t term2 = S2_prev[k - 1];
            S2_curr[k] = term1 + term2;
        }

        /* A(n,k) recurrence using row n-2 (A_prev2) */
        for (int k = 0; k <= MAXK; ++k)
        {
            seq_t part1 = (seq_t)k * A_prev2[k];
            seq_t part2 = (k >= 1) ? A_prev2[k - 1] : (seq_t)0;
            seq_t part3 = (k >= 2) ? A_prev2[k - 2] : (seq_t)0;
            A_curr[k] = part1 + part2 + part3;
        }

        /* If we have reached the requested n, compute a(n). */
        if (i == n)
        {
            /* difference is guaranteed non‑negative */
            seq_t diff = S2_curr[K] - A_curr[K];
            result = diff / (seq_t)2;
        }

        /* Shift rows for next iteration */
        for (int k = 0; k <= MAXK; ++k)
        {
            S2_prev[k] = S2_curr[k];
            A_prev2[k] = A_prev1[k];
            A_prev1[k] = A_curr[k];
        }
    }

    return result;
}
