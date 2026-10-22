/*
 * A366977  Array T(n,k), n >= 1, k >= 0, read by antidiagonals downwards,
 *          where T(n,k) = Sum_{j=1..n} binomial(floor(n/j)+k,k+1).
 * https://oeis.org/A366977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=1..n} binomial(floor(n/j)+k, k+1)
 * Range:     u8 n <= 50, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            where the sequence a(n) enumerates the table T(n,k) by antidiagonals
 *            with n>=1, k>=0:  (n,k) = (s‑j, j‑1) for each diagonal s. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Find the integer s such that s*(s+1)/2 >= N,
 * i.e. the diagonal containing the Nth term.  */
static inline idx_t find_diagonal(idx_t N)
{
    idx_t low = 1, high = 1;
    /* Expand high until the triangular number ≥ N */
    while ((unsigned __int128)high * (high + 1ULL) / 2ULL < N) {
        high *= 2;
    }
    /* Binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1ULL) / 2ULL;
        if (t >= N)
            high = mid;
        else
            low = mid + 1;
    }
    return low;   /* low == high */
}

seq_t a(idx_t N)
{
    /* Locate the diagonal s = n + k for the Nth entry */
    idx_t s = find_diagonal(N);

    /* Total number of elements before this diagonal */
    idx_t prev_sum = (s - 1ULL) * s / 2ULL;

    /* Offset inside the diagonal (0‑based) */
    idx_t offset = N - prev_sum - 1ULL;

    /* Corresponding table coordinates */
    idx_t n_val = offset + 1ULL;        /* n >= 1 */
    idx_t k_val = s - n_val;            /* k >= 0 */

    /* Compute T(n_val, k_val) using the formula
     *   T(n,k) = Σ_{j=1..n} binomial(j+k-1, k) * floor(n/j)     */
    seq_t result = 0;
    seq_t binom_val = 1;                 /* binom(j+k-1, k) for j = 1 */

    for (idx_t j = 1; j <= n_val; ++j) {
        seq_t floor_div = (seq_t)(n_val / j);
        result += binom_val * floor_div;

        if (j < n_val) {
            /* binom(j+1 + k_val -1, k_val) = binom(j + k_val, k_val)
             *   = binom(j + k_val -1, k_val) * (j + k_val) / j         */
            binom_val = binom_val * (seq_t)(j + k_val) / (seq_t)j;
        }
    }

    return result;
}
