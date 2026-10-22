/*
 * A372311  Triangle read by rows: T(n, k) = n^k * Sum_{j=0..n} binomial(n -
 *          j, n - k) * Eulerian1(n, j).
 * https://oeis.org/A372311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T(n,k) read by rows with
 *            T(r,k) = r^k * Σ_{j=0..r} binomial(r - j, r - k) * Eulerian1(r,
 *            j) where Eulerian1(r,j) are the Eulerian numbers of the first
 *            kind. The triangle rows are indexed starting at 0, and a(0) =
 *            T(0,0)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Determine row r and column k such that
       n = r*(r+1)/2 + k,  0 ≤ k ≤ r. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate arrays for Eulerian numbers of rows 0..r. */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;   /* Allocation failed – return a safe value. */
    }

    /* Row 0: E(0,0)=1, others 0. */
    prev[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i) prev[i] = (seq_t)0;

    /* Build Eulerian numbers up to row r using the recurrence */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            seq_t term1 = 0, term2 = 0;
            if (j <= i - 1) {
                term1 = ((seq_t)(j + 1)) * prev[j];
            }
            if (j >= 1) {
                term2 = ((seq_t)(i - j)) * prev[j - 1];
            }
            curr[j] = term1 + term2;
        }
        /* Zero unused entries beyond current row length. */
        for (idx_t j = i + 1; j <= r; ++j) {
            curr[j] = (seq_t)0;
        }
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Compute the weighted sum Σ_{j=0..min(k,r‑1)} binomial(r‑j, r‑k) * E(r,j). */
    seq_t sum = (seq_t)0;
    idx_t bound = (k < r) ? k : r;
    for (idx_t j = 0; j <= bound; ++j) {
        seq_t b = binomial(r - j, r - k);
        sum += b * prev[j];
    }

    /* Compute r^k. */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        power *= (seq_t)r;
    }

    seq_t result = power * sum;

    free(prev);
    free(curr);
    return result;
}
