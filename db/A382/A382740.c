/*
 * A382740  Square array A(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where A(n,k) = n! * k! * [x^n * y^k] (1/2) * (1 /
 *          (exp(x) + exp(y) - exp(x+y))^2 - 1).
 * https://oeis.org/A382740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read anti‑diagonally from the square array A(n,k)
 *            defined by A(n,k) = (1/2) * Σ_{m=1}^{min(n,k)}
 *            (m+1)·(m!)²·S(n,m)·S(k,m), where S(n,m) are Stirling numbers of
 *            the second kind
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int initialized = 0;
#define MAX_N 200

static seq_t fact[MAX_N + 1];
static seq_t stir[MAX_N + 1][MAX_N + 1];

/* Pre‑compute factorials and Stirling numbers of the second kind. */
static void
init(void)
{
    if (initialized)
        return;
    initialized = 1;

    /* factorials */
    fact[0] = 1;
    for (idx_t i = 1; i <= MAX_N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Stirling numbers S(n,k) for 0 ≤ n,k ≤ MAX_N */
    stir[0][0] = 1;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        stir[n][0] = 0;
        for (idx_t k = 1; k <= n; ++k)
            stir[n][k] = stir[n - 1][k - 1] + (seq_t)k * stir[n - 1][k];
    }
}

/* Compute the A(n,k) value using the formula above. */
static seq_t
A_value(idx_t n, idx_t k)
{
    if (n > MAX_N || k > MAX_N) {
        /* Should not happen for the intended range, but guard against overflow. */
        return 0;
    }

    idx_t min_m = (n < k) ? n : k;
    seq_t total = 0;
    for (idx_t m = 1; m <= min_m; ++m) {
        seq_t term = fact[m];               /* m!          */
        term = term * term;                 /* (m!)²       */
        term = term * stir[n][m];           /* * S(n,m)    */
        term = term * stir[k][m];           /* * S(k,m)    */
        term = term * (seq_t)(m + 1);       /* * (m+1)     */
        total = total + term;
    }
    /* Divide by 2.  The result is always an integer. */
    return total / (seq_t)2;
}

/* Main sequence function as requested. */
seq_t
a(idx_t n)
{
    /* Ensure tables are ready. */
    init();

    /* Find the anti‑diagonal index s such that (s‑1)*s/2 >= n. */
    idx_t s = 2;
    while (((s - 1) * s) / 2 < n)
        ++s;

    /* Sum of the previous diagonals. */
    idx_t prev_sum = ((s - 2) * (s - 1)) / 2;

    /* Position m on this diagonal (1‑based). */
    idx_t m = n - prev_sum;

    /* Corresponding indices in the square array. */
    idx_t n_idx = m;
    idx_t k_idx = s - m;

    return A_value(n_idx, k_idx);
}
