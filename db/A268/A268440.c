/*
 * A268440  Triangle read by rows, T(n,k) = C(2*n,n+k)*Sum_{m=0..k}
 *          (-1)^(m+k)*C(n+k,n+m)* Stirling1(n+m,m), for n>=0 and 0<=k<=n.
 * https://oeis.org/A268440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) of OEIS A268440:
 *   T(n,k) = binom(2n,n+k) *
 *            Σ_{m=0..k} (-1)^(m+k) * binom(n+k,n+m) * S1(n+m,m)
 *   where S1(n,m) is the unsigned Stirling number of the first kind.
 * The flattened sequence a(idx) is obtained by reading the triangle row‑by‑row:
 *   r = floor((√(8*idx+1)-1)/2),  c = idx - r(r+1)/2   (0‑based).
 *   a(idx) = T(r,c).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ---------- binomial coefficient -------------------------------------- */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

/* ---------- compute T(n,k) -------------------------------------------- */
static seq_t compute_T(idx_t n, idx_t k)
{
    /* ---------- Stirling numbers for current N = n ------------------- */
    seq_t *S = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (!S) return 0; /* fallback on OOM: should not happen */

    /* DP to fill S[j] = S1(n,j)  (0 ≤ j ≤ k)                  */
    for (idx_t j = 0; j <= k; ++j) S[j] = 0;
    S[0] = 1;                         /* S1(0,0) = 1  (N=0)  */
    for (idx_t N = 1; N <= n; ++N) {   /* build rows up to N = n */
        idx_t limit = (N < k) ? N : k;
        for (idx_t j = limit; j >= 1; --j)
            S[j] = S[j-1] + (seq_t)(N-1) * S[j];
        S[0] = 0;
    }

    /* ---------- inner sum Σ_{m} (-1)^(m+k) * C(n+k,n+m) * S1(n+m,m) ---- */
    seq_t sum_pos = 0, sum_neg = 0;
    seq_t binom_sum = binomial(n + k, n);   /* m = 0 : C(n+k,n)          */
    idx_t current_N = n;                    /* current N in S array          */

    for (idx_t m = 0; m <= k; ++m) {
        seq_t term = binom_sum * S[m];
        if (((m + k) & 1) == 0)
            sum_pos += term;   /* (+)               */
        else
            sum_neg += term;   /* (−)               */

        if (m < k) {
            /* update S to represent S1(current_N+1, j)   */
            for (idx_t j = k; j >= 1; --j)
                S[j] = S[j-1] + (seq_t)current_N * S[j];
            S[0] = 0;
            /* update binomial coefficient for next m      */
            binom_sum = binom_sum * (seq_t)(k - m) /
                        (seq_t)(current_N + 1);
            ++current_N;
        }
    }
    free(S);

    seq_t inner_sum = sum_pos - sum_neg;   /* guaranteed non-negative */
    seq_t binom2 = binomial(2 * n, n + k);
    return binom2 * inner_sum;
}

/* ---------- public entry ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Convert flattened index n into row r and column c */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1.0) / 2.0);
    /* Correct any floating‑point rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t c = n - r * (r + 1) / 2;   /* 0 ≤ c ≤ r */

    return compute_T(r, c);
}
