/*
 * A391950  Triangle T(n,k) of coefficients of polynomials arising from
 *          Gaussian-weighted Chebyshev integrals.
 * https://oeis.org/A391950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A391950: Triangle of coefficients of Gaussian‑weighted Chebyshev integrals
   Recurrence (OEIS):
   T(n,k) = 2*δ(n‑1,k) – T(n‑1,k‑1) + 2*(2n‑1) * Σ_{j=1}^{n‑1} T(j,k‑(n‑j‑1)),
   where 0 ≤ k < n and T(n,k)=0 otherwise.
   The sequence a(n) is a–the flattened triangle: row 1, row 2, … .
   This implementation builds the triangle iteratively, caching all terms.
   It works for any seq_t (unsigned integer or arbitrary‑precision) and
   any idx_t (uint64_t).  The code uses only integer arithmetic,
   dynamic allocation, and standard C‑library functions.
*/

#include <stdlib.h>

/* --- static cache for flattened triangle values ---------------------------------- */

static seq_t *seq_cache = NULL;   /* flattened values */
static size_t cache_len = 0;      /* number of stored values  */
static size_t cache_cap = 0;      /* capacity of seq_cache   */

static void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = val;
}

/* --- data for the current row of P(n) and the cumulative S(n) -------------------- */

static seq_t *row_prev = NULL; /* coefficients of P_{n-1}  (length current_row) */
static idx_t  row_len  = 0;    /* number of coefficients in row_prev         */
static seq_t *S_prev   = NULL; /* coefficients of S_{n-1} (length current_row-1)*/
static idx_t  S_len    = 0;    /* number of coefficients in S_prev            */
static idx_t  current_row = 0; /* highest n that has been generated so far     */

/* --- compute the next row of the triangle ------------------------------------- */

static void compute_next_row(void)
{
    idx_t n = current_row + 1; /* new row number, 1‑based */

    if (n == 1) { /* first row: P_1(x) = 2x */
        if (row_prev) free(row_prev);
        if (S_prev)  free(S_prev);

        row_prev = (seq_t*)calloc(1, sizeof(seq_t));
        row_prev[0] = (seq_t)2;
        row_len = 1;
        S_prev = NULL;
        S_len  = 0;

        push_cache(row_prev[0]); /* a(1) = 2 */
        current_row = n;
        return;
    }

    /* allocate space for the new row and for the new S(n) values */
    seq_t *S_curr = (seq_t*)calloc((size_t)(n-1), sizeof(seq_t));
    seq_t *P_curr = (seq_t*)calloc((size_t)n, sizeof(seq_t));

    /* build S_curr from S_prev and row_prev:
         S_curr[0] = row_prev[0]
         S_curr[k] = S_prev[k-1] + row_prev[k]  for 1 ≤ k ≤ n-2
    */
    S_curr[0] = row_prev[0];
    for (idx_t k = 1; k < n-1; ++k) {
        S_curr[k] = S_prev[k-1] + row_prev[k];
    }

    /* constant factor 2*(2n-1), fits in idx_t and can be safely cast to seq_t */
    seq_t factor = (seq_t)(2 * (2 * n - 1));

    /* compute P_curr coefficients */
    /* k = 0  */
    P_curr[0] = S_curr[0] * factor;

    /* 1 ≤ k ≤ n-2  */
    for (idx_t k = 1; k < n-1; ++k) {
        P_curr[k] = S_curr[k] * factor - row_prev[k-1];
    }

    /* last coefficient (k = n-1) */
    P_curr[n-1] = (seq_t)2 - row_prev[n-2];

    /* append the new row to the flattened cache */
    for (idx_t k = 0; k < n; ++k) {
        push_cache(P_curr[k]);
    }

    /* free old memory and finalize the new row as the current one */
    if (row_prev) free(row_prev);
    if (S_prev)   free(S_prev);

    row_prev = P_curr;
    row_len  = n;
    S_prev   = S_curr;
    S_len    = n-1;
    current_row = n;
}

/* --- public entry point -------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* by definition (offset 1) */
    while (cache_len < (size_t)n) {
        compute_next_row();
    }
    return seq_cache[n-1];
}
