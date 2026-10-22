/*
 * A325139  Triangle T(n, k) = [t^n] Gamma(n + k + m + t)/Gamma(k + m + t) for
 *          m = 2 and 0 <= k <= n, read by rows.
 * https://oeis.org/A325139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A325139: T(n,k) = Σ_{j=0..n-k} binomial(j+k,k) * |Stirling1(n,j+k)|
   with m = 2.  a(n) is the row‑major enumeration of T, offset 0. */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t n, idx_t k); */

static seq_t *cache = NULL;          /* row‑major array of T(n,k)   */
static idx_t cache_len = 0;          /* number of entries in cache  */

/* recompute the triangle up to the largest row needed to cover index nmax
   (i.e. the smallest r with r*(r+1)/2 > nmax).  All entries are stored in
   cache[0 .. cache_len-1] . */
static void recompute(idx_t nmax)
{
    /* Figure out how many rows are required so that the total number of
       entries up to that row strictly exceeds nmax.  This solves
       r*(r+1)/2 > nmax.  The safe way is to use the double arithmetic
       above; any rounding error only makes us do one extra row. */
    double rt = sqrt((double)(1 + 8.0 * (double)nmax));
    idx_t rows_needed = (idx_t)((rt - 1.0) / 2.0);

    /* Total number of triangle entries up to rows_needed inclusive */
    idx_t total = (rows_needed + 1) * (rows_needed + 2) / 2;

    /* Free previous cache if any */
    if (cache) free(cache);
    cache = (seq_t *)malloc((size_t)total * sizeof(seq_t));
    cache_len = total;

    /* Allocate a table of unsigned Stirling numbers of the first kind
       up to rows_needed.  Each row r will hold indices 0 … r
       (inclusive).  We allocate one extra slot (r+2) to avoid an
       out‑of‑bounds read for S[r-1][r] when r=1. */
    seq_t **S = (seq_t **)malloc((size_t)(rows_needed + 1) * sizeof(seq_t *));
    for (idx_t r = 0; r <= rows_needed; ++r) {
        S[r] = (seq_t *)calloc((size_t)(r + 2), sizeof(seq_t));
    }

    S[0][0] = 1;  /* base case: s(0,0) = 1 */

    for (idx_t r = 1; r <= rows_needed; ++r) {
        S[r][0] = 0;              /* s(r,0) = 0 for r>0 */
        for (idx_t k = 1; k <= r; ++k) {
            seq_t over_term = 0;
            if (k <= r - 1) {      /* S[r‑1][k] exists only for k ≤ r‑1 */
                over_term = ((seq_t)(r - 1)) * S[r - 1][k];
            }
            S[r][k] = S[r - 1][k - 1] + over_term;
        }
    }

    /* Compute the triangle entries and store them in row‑major order. */
    idx_t offset = 0;
    for (idx_t r = 0; r <= rows_needed; ++r) {
        for (idx_t k = 0; k <= r; ++k) {
            seq_t val = 0;
            seq_t pow_term = 1;   /* (k+2)^0 */
            for (idx_t j = 0; j <= r - k; ++j) {
                if (j > 0) pow_term *= (seq_t)(k + 2);
                seq_t coeff = binom(j + k, k);
                seq_t term  = coeff * S[r][j + k];
                term *= pow_term;
                val += term;
            }
            cache[offset + k] = val;
        }
        offset += r + 1;
    }

    /* Release the Stirling table. */
    for (idx_t r = 0; r <= rows_needed; ++r) free(S[r]);
    free(S);
}

seq_t a(idx_t n)
{
    if (n >= cache_len) recompute(n);
    return cache[n];
}
