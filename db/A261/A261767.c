/*
 * A261767  Triangle read by rows: T(n,k) is the number of subpermutations of
 *          an n-set, whose orbits are each of size at most k with at least
 *          one orbit of size exactly k.
 * https://oeis.org/A261767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   T(n,k) = A261763(n,k) – A261763(n,k‑1). A261763(n,k) counts
 *            partial injections (partial permutations) of an n‑set whose
 *            orbits all have size ≤ k. The recurrence below computes
 *            U(n,k)=A261763(n,k) with U(0,k)=1 and for n>0 U(n,k) =
 *            2·U(n‑1,k) + Σ_{l=2}^{min(k,n)} C(n‑1,l‑1)·(l!+(l‑1)!)·U(n‑l,k)
 *            (f...
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *fact = NULL;          /* factorial array, fact[i] = i!   */
static size_t fact_cap = 0;         /* current allocated capacity   */
static seq_t *dp   = NULL;          /* temporary DP array for U()   */
static size_t dp_cap = 0;           /* its capacity                */

/* ---------------------------------------------------------------------- */
/* Ensure that factorial array has length at least n+1.                    */
static void ensure_fact(idx_t n)
{
    if (n + 1 <= fact_cap) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_f = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
    if (!new_f) exit(EXIT_FAILURE);
    fact = new_f;
    /* compute new factorial entries */
    for (size_t i = fact_cap; i < new_cap; ++i)
        fact[i] = (i == 0 ? (seq_t)1 : fact[i-1] * (seq_t)i);
    fact_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/* Compute U(n,k) – the number of partial injections on n elements whose
   orbits all have size at most k.  Assumes n>=0, k>=0.                  */
static seq_t compute_U(idx_t n, idx_t k)
{
    if (n == 0)            return (seq_t)1;   /* U(0,k)=1            */
    if (k == 0)            return (seq_t)1;   /* U(n,0)=1 for all n  */

    if (n + 1 > dp_cap) {                     /* grow DP array         */
        seq_t *new_dp = (seq_t*)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_dp) exit(EXIT_FAILURE);
        dp = new_dp;
        dp_cap = (size_t)(n + 1);
    }

    dp[0] = (seq_t)1;

    for (idx_t t = 1; t <= n; ++t) {
        seq_t val = (seq_t)2 * dp[t-1];          /* isolated or 1‑cycle   */
        idx_t limit = (k < t ? k : t);

        if (limit >= 2) {                       /* contributions l≥2     */
            seq_t binom = (seq_t)(t-1);          /* C(t‑1,1) for l=2      */
            for (idx_t l = 2; l <= limit; ++l) {
                /* term = binom * (fact[l] + fact[l-1]) * dp[t‑l] */
                seq_t term = binom * (fact[l] + fact[l-1]) * dp[t-l];
                val += term;
                if (l < limit)                  /* next binomial coeff   */
                    binom = binom * (seq_t)(t - l) / (seq_t)l;
            }
        }

        dp[t] = val;
    }
    return dp[n];
}

/* ---------------------------------------------------------------------- */
/* Compute the row r for a flattened index idx.  Returns 0‑based row.      */
static inline idx_t row_of(idx_t idx)
{
    /* Solve r*(r+1)/2 <= idx < (r+1)*(r+2)/2 */
    long double d = sqrt((long double)(2.0L * (long double)idx + 1.0L));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    /* Adjust if due to rounding errors */
    while ((r + 1) * (r + 2) / 2 <= idx) ++r;
    while (r * (r + 1) / 2 > idx) --r;
    return r;
}

/* ---------------------------------------------------------------------- */
/* Public entry point required by the build system.                       */
seq_t a(idx_t n)
{
    /* Convert flattened index to triangle coordinates (row,col).   */
    idx_t row = row_of(n);
    idx_t col = n - row * (row + 1) / 2;      /* column in this row */

    if (row == 0)         /* a(0) = 1 */
        return (seq_t)1;

    if (col == 0)         /* T(row,0) is always 1 */
        return (seq_t)1;

    /* Ensure factorials up to 'row' are available. */
    ensure_fact(row);

    /* Compute U(row,col) and U(row,col‑1). */
    seq_t U_col     = compute_U(row, col);
    seq_t U_col_min = compute_U(row, col - 1);

    return U_col - U_col_min;
}
