/*
 * A384968  Triangle read by rows: T(n,k) is the number of proper vertex
 *          colorings of the n-complete bipartite graph using exactly k
 *          interchangeable colors, 2 <= k <= 2*n.
 * https://oeis.org/A384968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(m,k) where m = floor_isqrt_u64(n-1)+1 (row number in
 *            the triangle) k = (n - (m-1)^2) + 1 (column number in the
 *            triangle, 2 ≤ k ≤ 2m) T(m,k) = Σ_{j=1}^{k-1} S(m,j)·S(m,k-j),
 *            S(m,j) = Stirling numbers of the second kind (S(0,0)=1, S(m,0)=0
 *            for m>0)
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 105, u64 n <= 236,
 *            u128 n <= 642, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------ */
/* Integer square‑root: floor(sqrt(x)) for unsigned 64‑bit x.  */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = mid + 1;
            continue;
        }
        if (mid <= x / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t pos)
{
    /* ----- locate row (m) and column (k) in the triangle ----- */
    idx_t m = floor_isqrt_u64(pos - 1) + 1;             /* row number, m ≥ 1 */
    idx_t sqprev = (m - 1) * (m - 1);                    /* (m-1)^2   */
    idx_t offset = pos - sqprev;                        /* 1‑based position inside the row  */
    idx_t k = offset + 1;                               /* 2 ≤ k ≤ 2m   */

    /* ----- allocate arrays for Stirling numbers S(m, j) ----- */
    size_t sz = (size_t)(m + 1);
    seq_t *S_prev = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *S_cur  = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!S_prev || !S_cur) {
        /* allocation failed – return 0 as a safe fallback */
        if (S_prev) free(S_prev);
        if (S_cur)  free(S_cur);
        return (seq_t)0;
    }

    /* ----- initial row S(0,0)=1 ----- */
    S_prev[0] = (seq_t)1;
    for (idx_t j = 1; j <= m; ++j) S_prev[j] = (seq_t)0;

    /* ----- compute S(i,j) up to i = m using recurrence ----- */
    for (idx_t i = 1; i <= m; ++i) {
        S_cur[0] = (seq_t)0;
        for (idx_t j = 1; j <= i; ++j) {
            S_cur[j] = S_prev[j-1] + (seq_t)j * S_prev[j];
        }
        for (idx_t j = i+1; j <= m; ++j) {
            S_cur[j] = (seq_t)0;
        }
        /* swap pointers for next iteration */
        seq_t *tmp = S_prev;
        S_prev = S_cur;
        S_cur  = tmp;
    }

    /* ----- evaluate T(m,k) = Σ S(m,j)·S(m,k-j) ----- */
    seq_t sum = (seq_t)0;
    idx_t j_start = 1;
    if (k > m + 1) j_start = k - m;          /* ensure k-j ≤ m */
    idx_t j_end = k - 1;
    if (j_end > m) j_end = m;
    for (idx_t j = j_start; j <= j_end; ++j) {
        idx_t jj = k - j;
        sum += S_prev[j] * S_prev[jj];
    }

    /* ----- clean up and return ----- */
    free(S_prev);
    free(S_cur);
    return sum;
}
