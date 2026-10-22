/*
 * A336233  a(n) is the player who has highest winning probability in the
 *          "Random Josephus Game" with n players.
 * https://oeis.org/A336233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes a(n), the index of a player with the
 *            highest winning probability in the Random Josephus Game with n
 *            players. The probabilities are obtained by the recurrence P_1(0)
 *            = 1 P_k(0) = P_{k-1}(k-2) P_k(i) = ((i-1)/(k-1)) * P_{k-1}(i-2)
 *            + ((k-1-i)/(k-1)) * P_{k-1}(i-1) for 1 ≤ i ≤...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* Allocate temporary arrays to hold probability vectors. */
    long double *prev = (long double *)malloc(sizeof(long double) * n);
    long double *next = (long double *)malloc(sizeof(long double) * n);
    if (!prev || !next) return (seq_t)0;   /* allocation failed */

    /* Base case: one player. */
    prev[0] = 1.0L;
    if (n == 1) {
        free(prev);
        free(next);
        return (seq_t)1;
    }

    /* Iterate from k = 2 up to n, filling probability vectors. */
    for (idx_t k = 2; k <= n; ++k) {
        /* index 0 of the new vector. */
        next[0] = prev[k - 2];          /* P_k(0) = P_{k-1}(k-2) */

        /* Remaining indices. */
        for (idx_t i = 1; i < k; ++i) {
            long double term1 = 0.0L;
            if (i >= 2) {
                term1 = ((long double)(i - 1)) / (long double)(k - 1) * prev[i - 2];
            }
            long double term2 = ((long double)(k - 1 - i)) / (long double)(k - 1) * prev[i - 1];
            next[i] = term1 + term2;
        }

        /* Prepare for next iteration. */
        long double *tmp = prev;
        prev = next;
        next = tmp;
    }

    /* Find the index of the maximum probability (break ties by larger index). */
    idx_t bestIdx = 0;
    long double bestProb = -1.0L;
    for (idx_t i = n; i-- > 0; ) {
        if (prev[i] > bestProb) {
            bestProb = prev[i];
            bestIdx = i;
        }
    }

    free(prev);
    free(next);

    return (seq_t)(bestIdx + 1);   /* convert to 1‑based player number */
}
