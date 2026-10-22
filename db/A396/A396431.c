/*
 * A396431  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where A(n,k) = [x^n] F_k(x)/x and F_k(x) is the k-th iteration of
 *          x*G4(x)^3 with G4(x) = 1 + x*G4(x)^4.
 * https://oeis.org/A396431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A396431
 *
 *  A square array A(n,k) (n,k ≥ 0) is defined by the recurrence
 *
 *        A(n,0) = [n==0 ? 1 : 0]
 *        A(n,k) = 3 * Σ_{j=0..n}
 *                     (j+1) * binomial(4n-j+3, n-j)/(4n-j+3) * A(j,k-1)
 *
 *  The sequence a(n) is obtained by reading the array by antidiagonals
 *  downwards:  a(n) = A(i,j) where i+j = t and n = t(t+1)/2 + i,
 *  with 0 ≤ i ≤ t ≤ j.
 *
 *  The implementation pre‑computes a square table of A(n,k) up to the
 *  required size for the requested index n.  The table size is always
 *  the smallest integer s such that s(s+1)/2 > n, guaranteeing that
 *  all needed entries are present.
 *
 *  The routine is O(s³) to fill the table, which for s ≈ 200 (needed
 *  up to n ≈ 20000) is negligible.  Subsequent calls reuse the table
 *  without recomputation, unless a larger n is requested.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *A = NULL;          /* flattened 2‑D array: A[n*s + k] */
static idx_t table_size = 0;     /* current side length of the table */

/* Compute the table of A(n,k) for 0 ≤ n,k < s */
static void compute_table(idx_t s)
{
    /* free old table if it exists */
    if (A) {
        free(A);
        A = NULL;
    }

    /* allocate new table */
    A = (seq_t *)malloc(s * s * sizeof(seq_t));
    if (!A) {
        /* allocation failed – abort */
        exit(1);
    }

    /* base column k = 0 */
    for (idx_t n = 0; n < s; ++n) {
        A[n * s + 0] = (n == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* fill remaining columns using the recurrence */
    for (idx_t k = 1; k < s; ++k) {
        for (idx_t n = 0; n < s; ++n) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= n; ++j) {
                idx_t top = 4 * n - j + 3;
                seq_t term = binomial(top, n - j);   /* C(top, n-j)  */
                term *= (seq_t)(j + 1);              /* * (j+1)      */
                term *= 3;                           /* * 3          */
                term /= (seq_t)top;                  /* / top        */
                term *= A[j * s + (k - 1)];          /* * A(j,k-1)   */
                sum += term;
            }
            A[n * s + k] = sum;
        }
    }

    table_size = s;
}

/* Return a(n) for the OEIS sequence A396431. */
seq_t a(idx_t n)
{
    /* Find the smallest s such that s(s+1)/2 > n  (s = t+1)   */
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 <= n) {
        ++t;
    }
    idx_t required_size = t + 1;

    /* Recompute the table if the current one is too small */
    if (required_size > table_size) {
        compute_table(required_size);
    }

    /* Determine the antidiagonal coordinates */
    idx_t offset = n - t * (t + 1) / 2;   /* offset within the diagonal  */
    idx_t i = offset;                    /* row index (n′)              */
    idx_t j = t - offset;                /* column index (k′)           */

    return A[i * table_size + j];
}
