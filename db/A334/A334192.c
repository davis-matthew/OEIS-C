/*
 * A334192  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals: A(n,k)
 *          = exp(1/k) * Sum_{j>=0} (k*j + 1)^n / ((-k)^j * j!).
 * https://oeis.org/A334192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) computed from the square array A(n,k) via the explicit
 *            recurrence derived from its exponential generating function:
 *            A(0,k) = 1, A(i,k) = - Σ_{j=0}^{i-1} C(i-1,j) * k^{i-j-1} *
 *            A(j,k) for i ≥ 1. Coordinates (i,k) are obtained from the
 *            antidiagonal index n: let d = floor((√(8n+1)-1)/2), i = n...
 * Range:     i8 n <= 32, i16 n <= 63, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Adjust the maximum i that can appear for n up to about 1e5.
   For n ≤ 10000, i never exceeds 150. */
#define MAX_I 200

static bool binom_init = false;
static seq_t C[MAX_I][MAX_I];

static void init_binom(void) {
    for (idx_t n = 0; n < MAX_I; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            if (k == 0 || k == n)
                C[n][k] = 1;
            else
                C[n][k] = C[n-1][k-1] + C[n-1][k];
        }
    }
    binom_init = true;
}

/* Integer square root for 64‑bit unsigned integer */
static idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == x)
            return mid;
        else if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n) {
    if (!binom_init)
        init_binom();

    /* Find the antidiagonal 'd' containing index n */
    idx_t t = 8 * n + 1;               /* 8n+1 fits in 64‑bit for n ≤ 10⁴       */
    idx_t sqrt_t = isqrt_u64(t);
    idx_t d = (sqrt_t - 1) / 2;         /* floor((√(8n+1)-1)/2)                    */
    idx_t prev = d * (d + 1) / 2;       /* number of terms before diagonal d     */
    idx_t i = n - prev;                 /* row index (i ≥ 0)                      */
    idx_t k = d - i + 1;                /* column index (k ≥ 1)                   */

    /* Powers of k up to i-1 */
    seq_t pow_k[MAX_I];
    pow_k[0] = 1;
    seq_t k_seq = (seq_t)k;
    for (idx_t e = 1; e <= i; ++e) {
        pow_k[e] = pow_k[e-1] * k_seq;
    }

    /* Compute A(0..i) for this k */
    seq_t A[MAX_I];
    A[0] = 1;
    for (idx_t m = 1; m <= i; ++m) {
        seq_t sum = 0;
        for (idx_t j = 0; j < m; ++j) {
            idx_t exp = m - j - 1;     /* exponent of k (≥1 for meaningful terms) */
            if (exp >= 1) {
                sum += C[m-1][j] * pow_k[exp] * A[j];
            }
        }
        A[m] = -sum;
    }
    return A[i];
}
