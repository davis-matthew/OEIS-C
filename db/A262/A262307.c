/*
 * A262307  Array read by antidiagonals: T(m,n) = number of m X n binary
 *          matrices with all 1's connected and no zero rows or columns.
 * https://oeis.org/A262307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 59, u64 n <= 125,
 *            u128 n <= 240, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = T(m,n) where (m,n) are the antidiagonal coordinates of the index n.
 *   T(m,n) = A183109(m,n)
 *            - Σ_{i=1}^{m-1} Σ_{j=1}^{n-1} T(i,j) * A183109(m-i,n-j)
 *                                      * C(m-1,i-1) * C(n,j)
 *
 *   A183109(m,n) = number of m×n binary matrices with no zero rows or columns:
 *   A183109(m,n) = Σ_{i=0}^{m} (-1)^i C(m,i) (2^{m-i} - 1)^n
 *
 *   C(n,k) = binomial coefficient.
 *
 *   The sequence is read by anti‑diagonals of the array T(m,n):
 *     a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), a(4)=T(1,3), …
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

/* Max dimension needed for n ≤ 10000: s ≈ 141 ⇒ m,n ≤ 140.  Handle up to 200 safely. */
#define MAX_DIMS 200
static bool initialized = false;
static idx_t current_max = 0;
static seq_t T[MAX_DIMS + 1][MAX_DIMS + 1];
static seq_t A[MAX_DIMS + 1][MAX_DIMS + 1];
static seq_t binom[MAX_DIMS + 1][MAX_DIMS + 1];
static seq_t pow2[MAX_DIMS * MAX_DIMS + 2];

static void precompute(idx_t needed)
{
    /* binomial table */
    for (idx_t n = 0; n <= needed; ++n) {
        binom[n][0] = binom[n][n] = (seq_t)1;
        for (idx_t k = 1; k < n; ++k) {
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
        }
    }

    /* powers of two */
    pow2[0] = (seq_t)1;
    for (idx_t e = 1; e <= needed*needed; ++e) {
        pow2[e] = pow2[e-1] * (seq_t)2;
    }

    /* A183109(m,n) for 1 ≤ m,n ≤ needed */
    for (idx_t m = 1; m <= needed; ++m) {
        for (idx_t n = 1; n <= needed; ++n) {
            seq_t pos = (seq_t)0;
            seq_t neg = (seq_t)0;
            for (idx_t i = 0; i <= m; ++i) {
                /* base = 2^{m-i} - 1 */
                seq_t base = pow2[m - i];
                base -= (seq_t)1;
                seq_t pow = seq_pow(base, n);
                seq_t term = binom[m][i] * pow;
                if ((i & 1) == 0)
                    pos += term;
                else
                    neg += term;
            }
            A[m][n] = pos - neg;
        }
    }

    /* T(m,n) by the recurrence */
    for (idx_t m = 1; m <= needed; ++m) {
        for (idx_t n = 1; n <= needed; ++n) {
            seq_t val = A[m][n];
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= m-1; ++i) {
                seq_t binom_m = binom[m-1][i-1];
                for (idx_t j = 1; j <= n-1; ++j) {
                    seq_t prod = T[i][j];
                    prod *= A[m-i][n-j];
                    prod *= binom_m;
                    prod *= binom[n][j];
                    sum += prod;
                }
            }
            T[m][n] = val - sum;
        }
    }

    current_max = needed;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range; behaviour not critical */

    /* Determine maximum indices needed for this n */
    idx_t s = 2;
    while ((s-1) * s / 2 < n) ++s;
    idx_t needed = s - 1;                 /* since m,n ≤ s-1 */

    if (!initialized || needed > current_max) precompute(needed);

    /* Find the coordinates (m, col) for this index n */
    idx_t prev_terms = (s-1) * (s-2) / 2;
    idx_t m = n - prev_terms;            /* 1‑based row index  */
    idx_t col = s - m;                   /* 1‑based column index */

    return T[m][col];
}
