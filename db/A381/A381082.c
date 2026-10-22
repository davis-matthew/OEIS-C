/*
 * A381082  Triangle T(n,k) read by rows, where the columns are the
 *          coefficients of the standard expansion of the function f(x) =
 *          (-log(1-x))^(k)*exp(-m*x)/k! for the case m=2.
 * https://oeis.org/A381082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8n+1)-1)/2) and c = n -
 *            r*(r+1)/2, T(r,c) = (-1)^(r-c) * sum_{j=c}^{r} s(j,c) *
 *            binomial(r,j) * 2^{r-j} s(j,c) are signed Stirling numbers of
 *            the first kind with s(0,0)=1
 * Range:     i8 n <= 28, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: compute unsigned integer floor of sqrt assuming x fits in 64-bit */
static idx_t _floor_isqrt_u64(idx_t n) {
    idx_t low = 0, high = (idx_t)1 << 32, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if ((idx_t)mid * (idx_t)mid <= n) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* Compute T(n,k) as described above */
static seq_t compute_T(idx_t n, idx_t k) {
    /* allocate array to store s(j,k) for j=0..n */
    seq_t *s_jk = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!s_jk) return 0;
    /* temporary arrays for DP over Stirling numbers */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * (k + 1));
    seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (k + 1));
    if (!prev || !curr) {
        free(s_jk);
        free(prev);
        free(curr);
        return 0;
    }
    /* init for j = 0 */
    for (idx_t t = 0; t <= k; ++t) prev[t] = 0;
    prev[0] = 1;
    s_jk[0] = (k == 0) ? 1 : 0;

    for (idx_t j = 1; j <= n; ++j) {
        curr[0] = 0;
        idx_t up = (j < k) ? j : k;
        for (idx_t t = 1; t <= up; ++t) {
            curr[t] = prev[t - 1] - (seq_t)(j - 1) * prev[t];
        }
        /* copy curr to prev for next iteration */
        for (idx_t t = 0; t <= up; ++t) prev[t] = curr[t];
        if (j >= k) s_jk[j] = curr[k];
        else s_jk[j] = 0;
    }

    /* compute binomial coefficients bin[j] = C(n,j) */
    seq_t *bin = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!bin) {
        free(prev);
        free(curr);
        free(s_jk);
        return 0;
    }
    bin[0] = 1;
    for (idx_t j = 1; j <= n; ++j) {
        bin[j] = bin[j - 1] * (n - j + 1) / j;
    }

    /* accumulate the sum with powers of two */
    seq_t sum = 0;
    seq_t pow2 = 1; /* corresponds to 2^{0} at j = n */
    idx_t jj = n;
    while (true) {
        seq_t term = s_jk[jj] * bin[jj];
        sum += term * pow2;
        pow2 <<= 1; /* multiply by 2 for next lower j */
        if (jj == 0) break;
        --jj;
    }

    free(prev);
    free(curr);
    free(bin);
    free(s_jk);

    /* apply overall sign factor (-1)^{n-k} */
    if (((n - k) & 1) != 0) sum = -sum;
    return sum;
}

seq_t a(idx_t n) {
    /* find row r and column c for flattened index n */
    idx_t r;
    /* binary search for r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t val = mid * (mid + 1) / 2;
        if (val == n) {
            r = mid;
            break;
        }
        if (val < n) {
            low = mid + 1;
            r = mid;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;
    return compute_T(r, c);
}
