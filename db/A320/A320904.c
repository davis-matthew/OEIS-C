/*
 * A320904  T(n, k) = binomial(2*n + 1 - k, k)*hypergeom([1, 1, -k], [1, 2*(n
 *          - k + 1)], -1), triangle read by rows, T(n, k) for n >= 0 and 0 <=
 *          k <= n.
 * https://oeis.org/A320904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A320904: triangle read by rows.
 * For each n ≥ 0 let r = floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2.
 * Let T(r,k) = binomial(2r+1-k, k) * 
 *   _3F2([1,1,-k],[1,2(r-k+1)],-1).
 * Then a(n) = T(r,k).  
 * The hypergeometric sum is evaluated by the recurrence
 *   term_0 = 1,
 *   term_{j+1} = term_j * (k-j) / (2(r-k+1)+j),
 *   S = Σ_{j=0}^k binomial(2r+1-k,k) * term_j,
 * which keeps all intermediate values integral.
 */
#include "../utils/binomial.h"

static idx_t find_row(idx_t n) {
    idx_t low = 0, high = 1;
    /* find an upper bound for r */
    while (high * (high + 1) / 2 <= n) high <<= 1;
    /* binary search for the greatest r with r(r+1)/2 ≤ n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t compute_T(idx_t r, idx_t k) {
    /* C = binomial(2r+1-k, k) */
    seq_t C = binomial(2 * r + 1 - k, k);
    seq_t sum = 0;
    seq_t term = C;          /* term_0 = C */
    sum += term;
    for (idx_t j = 0; j < k; ++j) {
        /* term_{j+1} = term_j * (k-j) / (2(r-k+1)+j) */
        seq_t numer = term * (seq_t)(k - j);
        seq_t denom = (seq_t)(2 * (r - k + 1) + j);
        term = numer / denom;   /* division is exact */
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* map position n to triangle coordinates (r,k) */
    idx_t r = find_row(n);
    idx_t k = n - r * (r + 1) / 2;
    return compute_T(r, k);
}
