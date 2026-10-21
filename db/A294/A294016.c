/*
 * A294016  a(n) = sum of all divisors of all positive integers <= n, minus
 *          the sum of remainders of n mod k, for k = 1, 2, 3, ..., n.
 * https://oeis.org/A294016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of divisors of all integers ≤ n) – (sum of
 *            remainders of n mod k, k=1..n)
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sigma_arr = NULL;       /* sigma[0..current_max] */
static seq_t *cum_sigma_arr = NULL;   /* cum_sigma[0..current_max] */
static idx_t cache_size = 0;

/* Recompute sigma and cumulative sigma up to N */
static void ensure_cache(idx_t N)
{
    if (N <= cache_size) return;

    /* free old arrays if any */
    if (sigma_arr) free(sigma_arr);
    if (cum_sigma_arr) free(cum_sigma_arr);

    /* allocate new arrays */
    sigma_arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    cum_sigma_arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!sigma_arr || !cum_sigma_arr) {
        /* Allocation failed – in a real build you might abort or set an error flag */
        exit(1);
    }

    /* Zero arrays */
    sigma_arr[0] = 0;
    for (idx_t i = 1; i <= N; ++i) sigma_arr[i] = 0;

    /* Sieve to compute sigma values */
    for (idx_t d = 1; d <= N; ++d) {
        seq_t sd = (seq_t)d;
        for (idx_t m = d; m <= N; m += d) {
            sigma_arr[m] += sd;
        }
    }

    /* Compute cumulative sigma */
    cum_sigma_arr[0] = 0;
    for (idx_t i = 1; i <= N; ++i) {
        cum_sigma_arr[i] = cum_sigma_arr[i - 1] + sigma_arr[i];
    }

    cache_size = N;
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    /* 1. Precompute up to n if needed */
    ensure_cache(n);

    /* 2. Compute the sum of remainders: R = n*n - sum_{k=1}^{n} k*floor(n/k) */
    seq_t nn = (seq_t)n;
    seq_t sum_kq = 0;  /* Σ k * floor(n/k) */

    idx_t i = 1;
    while (i <= n) {
        idx_t q = n / i;                /* floor(n / i) */
        idx_t last = n / q;             /* largest k with same quotient q */
        /* Sum of integers from i to last: (i + last) * (last - i + 1) / 2 */
        seq_t cnt = (seq_t)(last - i + 1);
        seq_t sum_k = (seq_t)(i + last) * cnt / 2;
        sum_kq += (seq_t)q * sum_k;
        i = last + 1;
    }

    seq_t R = nn * nn - sum_kq;          /* Σ (n mod k) */

    /* 3. a(n) = cumulative sigma[n] - R */
    seq_t result = cum_sigma_arr[n] - R;
    return result;
}
