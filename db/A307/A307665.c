/*
 * A307665  A(n,k) = Sum_{j=0..floor(n/k)} binomial(2*n,k*j+n), square array
 *          A(n,k) read by antidiagonals, for n >= 0, k >= 1.
 * https://oeis.org/A307665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=0..⌊n/k⌋} binomial(2*n, k*j+n) where (n,k) are the
 *            coordinates of the square array A(n,k) (n≥0, k≥1) read by
 *            antidiagonals (n is the row index, k the column index)
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utility binomial function is provided by the build. */
#include "../utils/binomial.h"
#include <stdint.h>   /* for UINT64_MAX */

static inline idx_t find_m(idx_t n)
{
    /* Return the diagonal index m such that
       m*(m+1)/2 <= n < (m+1)*(m+2)/2.  */
    if (n == 0) return 0;
    idx_t lo = 0;
    idx_t hi = 1;
    while (1) {
        __uint128_t val = ((__uint128_t)hi * (hi + 1u)) / 2u;
        if (val > n) break;
        if (hi > (UINT64_MAX >> 1)) {   /* cannot double without overflow */
            lo = hi;
            break;
        }
        hi <<= 1u;
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t val = ((__uint128_t)mid * (mid + 1u)) / 2u;
        if (val <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Map the global index n to the coordinates (row, column)
       of the antidiagonal reading of the square array.  */
    idx_t m = find_m(n);                           /* diagonal index   */
    idx_t prev = ((__uint128_t)m * (m + 1u)) / 2u;   /* elements before diag m */
    idx_t pos = n - prev;                          /* position inside diag m (0‑based) */
    idx_t n_value = pos;                           /* row index (0‑based) */
    idx_t k_value = m - pos + 1u;                  /* column index (1‑based) */

    /* Compute A(n_value, k_value) with the given summation formula. */
    seq_t result = 0;
    idx_t two_n = n_value << 1u;                    /* 2*n_value */
    idx_t jmax  = n_value / k_value;               /* floor(n/k) */
    for (idx_t j = 0; j <= jmax; ++j) {
        idx_t term_n = k_value * j + n_value;      /* argument of binomial */
        result += binomial(two_n, term_n);
    }
    return result;
}
