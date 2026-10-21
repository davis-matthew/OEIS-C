/*
 * A271879  Triangle T(n,t) by rows: The number of rooted forests with n
 *          3-colored nodes and t rooted trees.
 * https://oeis.org/A271879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 78, u64 n <= 276, u128 n <= 990,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A271879
 *
 *  Triangle T(n,t) by rows: the number of rooted forests with n
 *  3‑colored nodes and t rooted trees.
 *
 *  The nth term is the term in the row‑major ordering of the triangle:
 *      a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …
 *
 *  The formula implemented below follows the recurrence of the
 *  published reference (eq. 27 in the paper).  The function first
 *  maps the flat index n to row r and column t, computes the
 *  sequence r[k] of rooted tree counts by the recurrence,
 *  pre‑computes the necessary multinomial coefficients,
 *  and finally uses a two‑dimensional DP to obtain the number of
 *  rooted forests of size r with t components.
 *
 *  Complexity:  O(r³) with r≈√(2n).  For n≤10000, r≤142,
 *  so the routine finishes well under one second.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* external types supplied by the build system */
typedef uint64_t idx_t;

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* --- map flat index to row r and column t --- */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;
    idx_t t = n - r * (r - 1) / 2;

    /* maximum size needed for trees is r */
    idx_t max_k = r;

    /* --- compute r_k for k = 1 .. max_k using the recurrence --- */
    seq_t *rk = (seq_t *)calloc((size_t)(max_k + 1), sizeof(seq_t));
    seq_t *B  = (seq_t *)calloc((size_t)(max_k + 1), sizeof(seq_t));
    if (!rk || !B) {
        free(rk);
        free(B);
        return 0;            /* allocation failure */
    }

    rk[1] = 3;                /* r[1] = 3 */

    /* initialise B: for m=1..max_k add 1 * r[1] to all multiples of 1 */
    for (idx_t j = 1; j <= max_k; ++j)
        B[j] += rk[1];

    for (idx_t n1 = 2; n1 <= max_k; ++n1) {
        /* compute r[n1] */
        seq_t sum = 0;
        for (idx_t k = 1; k <= n1 - 1; ++k)
            sum += B[k] * rk[n1 - k];
        rk[n1] = sum / (seq_t)(n1 - 1);

        /* update B for later indices: add n1 * r[n1] to each multiple */
        for (idx_t j = n1; j <= max_k; j += n1)
            B[j] += (seq_t)n1 * rk[n1];
    }

    /* free B – no longer needed */
    free(B);

    /* --- pre‑compute binomials C_k[c] = binomial(rk[k]+c−1 , c) --- */
    seq_t **C = (seq_t **)malloc((size_t)(max_k + 1) * sizeof(seq_t *));
    if (!C) {
        free(rk);
        return 0;
    }
    for (idx_t k = 1; k <= max_k; ++k) {
        size_t sz = (size_t)(max_k + 1);
        C[k] = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!C[k]) {
            for (idx_t x = 1; x < k; ++x) free(C[x]);
            free(C);
            free(rk);
            return 0;
        }
        C[k][0] = 1;
        for (idx_t c = 1; c <= max_k; ++c) {
            seq_t num = rk[k] + (seq_t)(c - 1);
            C[k][c] = C[k][c - 1] * num / (seq_t)c;
        }
    }

    /* --- DP over k to build forest counts --- */
    size_t M = (size_t)max_k;
    size_t stride = M + 1;
    size_t cells  = stride * stride;
    seq_t *dp    = (seq_t *)calloc(cells, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(cells, sizeof(seq_t));
    if (!dp || !newdp) {
        free(rk);
        for (idx_t k = 1; k <= max_k; ++k) free(C[k]);
        free(C);
        free(dp);
        free(newdp);
        return 0;
    }
    dp[0] = 1;   /* empty forest */

    for (idx_t k = 1; k <= max_k; ++k) {
        /* clear newdp */
        for (size_t i = 0; i < cells; ++i) newdp[i] = 0;

        for (size_t n0 = 0; n0 <= M; ++n0) {
            for (size_t t0 = 0; t0 <= M; ++t0) {
                seq_t cur = dp[n0 * stride + t0];
                if (!cur) continue;
                size_t rem = M - n0;
                size_t max_c = rem / k;
                for (size_t c = 0; c <= max_c; ++c) {
                    size_t newn = n0 + c * k;
                    size_t newt = t0 + c;
                    seq_t mult = C[k][c];
                    newdp[newn * stride + newt] += cur * mult;
                }
            }
        }
        /* swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[(size_t)r * stride + (size_t)t];

    /* free memory */
    free(dp);
    free(newdp);
    for (idx_t k = 1; k <= max_k; ++k) free(C[k]);
    free(C);
    free(rk);

    return result;
}
