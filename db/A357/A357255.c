/*
 * A357255  Triangular array: row n gives the recurrence coefficients for the
 *          sequence (c(k) = number of subsets of {1,2,...,n} that have at
 *          least k-1 elements) for k >= 1.
 * https://oeis.org/A357255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   T(r,k) = (-1)^(k-1) * (C(r,k) + C(r-1,k-1))
 * Range:     i8 n <= 30, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the flattening of the triangular array: a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), ... */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;
    /* Compute column k (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;  /* 1 <= k <= r */

    /* Compute the binomial coefficient C(r,k) */
    seq_t val = binom(r, k);          /* C(r,k) */
    val = val * (seq_t)(r + k) / (seq_t)r;  /* multiply by (r+k) and divide by r */

    /* Apply the sign (-1)^(k-1), i.e., negate if k is even */
    if ((k & 1u) == 0)
        val = -val;

    return val;
}
