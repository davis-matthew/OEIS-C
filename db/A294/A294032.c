/*
 * A294032  Triangle read by rows, T(n, k) = Pochhammer(3, k)*Stirling2(3 + n,
 *          3 + k) for n >= 0 and 0 <= k <= n.
 * https://oeis.org/A294032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2) and c = n -
 *            r*(r+1)/2 T(r,c) = Pochhammer(3,c) * Stirling2(3+r, 3+c)
 *            Pochhammer(3,c) = (c+2)! / 2 Stirling2(n,k) is computed by the
 *            recurrence S(0,0)=1, S(n,0)=0, S(0,k)=0 for k>0,
 *            S(n,k)=k*S(n-1,k)+S(n-1,k-1) (indexed by n,k >= 0)
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 59, u64 n <= 163, u128 n <= 480,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* use the provided helper that computes factorial (idx_t) */
#include "../utils/factorial.h"

static seq_t *stirling = NULL;          /* flattened 2D table S(n,k) */
static idx_t max_stir_n = 0;            /* maximum n (rows) currently stored */
static idx_t max_stir_k = 0;            /* maximum k (cols) currently stored */

/* allocate a new stirling table up to n_max, k_max (both inclusive) */
static void compute_stirling_table(idx_t n_max, idx_t k_max) {
    size_t rows = (size_t)(n_max + 1);
    size_t cols = (size_t)(k_max + 1);
    size_t total = rows * cols;
    seq_t *new_tbl = (seq_t*)malloc(total * sizeof(seq_t));
    if (!new_tbl) return; /* abort on out‑of‑memory; caller will handle error */

    /* initialize all to zero */
    for (size_t i = 0; i < total; ++i) new_tbl[i] = 0;

    /* base case S(0,0)=1 */
    new_tbl[0 * cols + 0] = 1;

    for (idx_t n = 1; n <= n_max; ++n) {
        for (idx_t k = 1; k <= n && k <= k_max; ++k) {
            seq_t term1 = new_tbl[(n - 1) * cols + k];
            seq_t term2 = new_tbl[(n - 1) * cols + (k - 1)];
            new_tbl[n * cols + k] = (k * term1) + term2;
        }
    }

    /* free previous table if any */
    free(stirling);

    stirling = new_tbl;
    max_stir_n = n_max;
    max_stir_k = k_max;
}

seq_t a(idx_t n) {
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) r++;
    r--;                                           /* correct overshoot */

    /* column index */
    idx_t prev_count = r * (r + 1) / 2;
    idx_t c = n - prev_count;

    /* ensure stirling table large enough up to n = 3+r, k = 3+c */
    idx_t need_n = 3 + r;
    idx_t need_k = 3 + c;
    if (need_n > max_stir_n || need_k > max_stir_k) {
        idx_t new_n = (need_n > max_stir_n) ? need_n : max_stir_n;
        idx_t new_k = (need_k > max_stir_k) ? need_k : max_stir_k;
        /* k cannot exceed n in the recurrence, so use new_n as new_k */
        if (new_k > new_n) new_k = new_n;
        compute_stirling_table(new_n, new_k);
    }

    /* compute Pochhammer(3,c) = (c+2)! / 2  */
    seq_t factorial_val = factorial(c + 2);
    seq_t pochhammer = factorial_val / 2;   /* exact division */

    /* retrieve Stirling number S(3+r, 3+c) */
    idx_t n_idx = 3 + r;
    idx_t k_idx = 3 + c;
    /* use flattened indexing */
    seq_t stirling_val = stirling[n_idx * (max_stir_k + 1) + k_idx];

    /* result */
    return pochhammer * stirling_val;
}
