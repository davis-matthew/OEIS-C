/*
 * A290125  Square array read by antidiagonals T(n,k) = sigma(k + n) -
 *          sigma(k) - n, with n>=0 and k>=1.
 * https://oeis.org/A290125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(n_k) where T(n,k) = sigma(k + n) - sigma(k) - n read
 *            from the square array by antidiagonals with n ≥ 0, k ≥ 1. This
 *            implementation computes the antidiagonal coordinates for a given
 *            index n, then evaluates T(n,k) using a simple divisor summation
 * Range:     i8 n <= 77, i16 n <= 5049, i32 n <= 5049, i64 n <= 5049,
 *            i128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the sum of divisors (σ) of a positive integer m. */
static inline seq_t sigma_of_idx(idx_t m)
{
    if (m == 0) return 0;
    seq_t sum = 0;
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            sum += (seq_t)d;
            idx_t other = m / d;
            if (other != d) sum += (seq_t)other;
        }
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal s such that n < s*(s+1)/2. */
    idx_t s = 0;
    while (((seq_t)s * ((seq_t)s + 1) / 2) <= (seq_t)n)
        ++s;

    /* Previous cumulative count: (s-1)*s/2. */
    idx_t prev = (s - 1) * s / 2;

    /* Position within this diagonal: n_val. */
    idx_t n_val = n - prev;          /* 0 ≤ n_val < s */
    idx_t k_val = s - n_val;         /* k ≥ 1 */

    /* Compute T(n_val, k_val). */
    seq_t sigma_sum = sigma_of_idx(n_val + k_val);
    seq_t sigma_k   = sigma_of_idx(k_val);

    return sigma_sum - sigma_k - (seq_t)n_val;
}
