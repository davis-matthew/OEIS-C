/*
 * A368028  Square array read by antidiagonals; T(n,k) = number of ways a
 *          vehicle with capacity k can transport n distinct individuals with
 *          distinct starting and finishing points.
 * https://oeis.org/A368028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array T(n,k) by antidiagonals. *
 *            T(n,k) = number of ways to transport n distinct individuals with
 *            a * vehicle of capacity k, as described in the OEIS entry. The
 *            sequence * is defined by the recurrence from f(n,k,c): * *
 *            f(0,k,c) = c! (for 0 ≤ c ≤ k) * f(n,k,c) = n*f(n...
 * Range:     u8 n <= 22, u16 n <= 37, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Compute T(n,k) = f(n,k,0) by dynamic programming                  */
static seq_t count_T(idx_t n, idx_t k)
{
    /* allocate arrays of length k+2 to allow access to c+1 safely */
    size_t sz = (size_t)(k + 2);
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !curr)
    {
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* initial row: f(0,k,c) = c! for 0 ≤ c ≤ k */
    prev[0] = (seq_t)1;
    for (idx_t c = 1; c <= k; ++c)
        prev[c] = prev[c - 1] * (seq_t)c;   /* factorial */

    /* iterate for each of the n individuals */
    for (idx_t step = 1; step <= n; ++step)
    {
        seq_t step_val = (seq_t)step;
        for (idx_t c = 0; c <= k; ++c)
        {
            seq_t part1 = 0;
            if (c + 1 <= k)
                part1 = step_val * prev[c + 1];
            seq_t part2 = 0;
            if (c > 0)
                part2 = (seq_t)c * curr[c - 1];
            curr[c] = part1 + part2;
        }
        /* swap the rows for the next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[0];   /* f(n,k,0) */
    free(prev);
    free(curr);
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
seq_t a(idx_t n)
{
    /* Locate the antidiagonal containing the index n */
    double root = sqrt((double)(8 * (double)n + 1.0));
    idx_t d = (idx_t)((root - 1.0) * 0.5);                /* floor */
    idx_t diag_start = d * (d + 1) / 2;                  /* first index on diagonal d */
    idx_t pos = n - diag_start;                          /* position within diagonal (0‑based) */

    /* Coordinates: i = d - pos, k = pos */
    idx_t i = d - pos;
    idx_t k = pos;

    return count_T(i, k);
}
