/*
 * A327031  A(n, k) = Sum_{d|n} phi(d)*T(n/d, k) if n > 0 and A(0, k) = 0
 *          where T(n, k) = binomial(n+k-1, n). Square array read by ascending
 *          antidiagonals, with n, k >= 0.
 * https://oeis.org/A327031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-01)
 * Helpers:   binomial, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A327031 (OEIS)
 *
 * The array A(n,k) (n,k ≥ 0) is defined by
 *
 *    A(0,k) = 0,
 *    A(n,k) = Σ_{d|n} φ(d) * binomial(n/d + k − 1 , n/d)   for n > 0
 *
 * The required 1‑dimensional sequence a(n) is read from the array by
 * ascending antidiagonals:
 *   index 0 → (0,0)
 *   index 1 → (1,0)
 *   index 2 → (0,1)
 *   index 3 → (2,0)
 *   ...
 *
 * This implementation does not hard‑code any values and supports any
 * integer type given for `idx_t` and `seq_t`.  All intermediate
 * computations are done using `seq_t` to guarantee exactness.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/totient.h"

/* ---------- compute A(row,col) for row > 0 ---------- */
static inline seq_t _A_value(idx_t row, idx_t col) {
    if (row == 0) return 0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= row; ++d) {
        if (row % d == 0) {
            /* divisor d */
            idx_t nd = row / d;
            seq_t phi_d = totient(d);
            seq_t term = binomial(nd + col - 1, nd);
            sum += phi_d * term;

            /* complement divisor nd (= row/d) if distinct */
            if (nd != d) {
                seq_t phi_nd = totient(nd);
                seq_t term2 = binomial(d + col - 1, d);
                sum += phi_nd * term2;
            }
        }
    }
    return sum;
}

/* ---------- integer square root (floor) for 64‑bit unsigned integers ---------- */
static inline idx_t _isqrt_u64(idx_t x) {
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t sq = ( __uint128_t)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* ---------- main function required by the build system ---------- */
seq_t a(idx_t n) {
    /* locate the point (row,col) in the antidiagonal indexing */
    if (n == 0) {
        /* (0,0) -> A(0,0) = 0 */
        return 0;
    }

    idx_t m = (idx_t)(((__uint128_t)8 * n) + 1);
    idx_t s = (_isqrt_u64(m) - 1) / 2;        /* diagonal index */
    idx_t prev = s * (s + 1) / 2;            /* count before this diagonal */
    idx_t j = n - prev;                      /* position on the diagonal */
    idx_t row = s - j;                       /* row index in the array */
    idx_t col = j;                           /* column index */

    return _A_value(row, col);
}
