/*
 * A396413  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where A(n,k) = [x^n] F_k(x)/x and F_k(x) is the k-th iteration of
 *          x*G4(x) with G4(x) = 1 + x*G4(x)^4.
 * https://oeis.org/A396413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   map_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A396413: square array A(n,k) read by antidiagonals downwards.
   Recurrence:  A(n,0) = 0^n  (1 if n=0, else 0)
   For k>0:
        A(n,k) = Σ_{j=0..n} (j+1) * binom(4*n-3*j+1, n-j) / (4*n-3*j+1) * A(j,k-1)
   The sequence a(n) is A(i,j) where (i,j) is the (n‑th) entry in the
   antidiagonal enumeration described above.
   The function expands a small 2‑D cache on demand and recomputes it
   whenever a larger index is requested.  This runs in O((√n)^2) time
   which is well below the limits for n up to 10 000.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/map_idx.h"

/* ---------- local binomial -------------------------------------- */
static seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* ---------- table handling --------------------------------------- */
static seq_t        *table = NULL;          /* 1‑D array holding A(n,k) */
static idx_t          n_dim = 0,           /* current max i used in table */
                      k_dim = 0;           /* current max j used in table */

/* Rebuilds the table to cover at least indices up to max_n and max_k. */
static bool rebuild_table(idx_t max_n, idx_t max_k)
{
    size_t new_size = (size_t)(max_n + 1) * (size_t)(max_k + 1);
    seq_t *new_tbl = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_tbl) return false;            /* malloc failed */

    /* Initialize column k=0 */
    for (idx_t n = 0; n <= max_n; ++n)
        new_tbl[n * (max_k + 1) + 0] = (n == 0) ? (seq_t)1 : (seq_t)0;

    /* Compute higher columns */
    for (idx_t k = 1; k <= max_k; ++k)
    {
        for (idx_t n = 0; n <= max_n; ++n)
        {
            seq_t sum = 0;
            for (idx_t j = 0; j <= n; ++j)
            {
                seq_t prev = new_tbl[j * (max_k + 1) + (k - 1)];
                if (prev == 0) continue;

                idx_t denom_val = 4 * n - 3 * j + 1;
                seq_t bin = binom_local((idx_t)denom_val, n - j);
                seq_t term = bin * (seq_t)(j + 1);
                term /= (seq_t)denom_val;
                term *= prev;
                sum += term;
            }
            new_tbl[n * (max_k + 1) + k] = sum;
        }
    }

    free(table);
    table = new_tbl;
    n_dim = max_n + 1;
    k_dim = max_k + 1;
    return true;
}

/* ---------- public function -------------------------------------- */
seq_t a(idx_t n)
{
    /* Find required coordinates */
    idx_t i, j;
    map_idx(n, &i, &j);

    /* If the current table is too small, enlarge it */
    if ((idx_t)(i + 1) > n_dim || (idx_t)(j + 1) > k_dim)
    {
        if (!rebuild_table(i, j)) return 0;   /* should not happen */
    }

    /* Return the requested entry */
    return table[i * k_dim + j];
}
