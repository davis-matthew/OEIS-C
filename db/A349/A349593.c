/*
 * A349593  Square array read by downward diagonals: for n >= 0, k >= 1,
 *          T(n,k) is the period of {binomial(N,n) mod k: N in Z}.
 * https://oeis.org/A349593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the period T(r,k) of {binomial(N,r) mod k : N∈ℤ} where
 *            the pair (r,k) is the n-th element in the square array read by
 *            downward diagonals: first the element with r=0,k=1 (diagonal 0),
 *            then the elements of diagonal 1 from top to bottom, then
 *            diagonal 2, etc. Thus if d = ⌊(√(8⋅n+1)-1)/2⌋ (at...
 * Range:     u8 n <= 113, u16 n <= 1509, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   log_floor, period_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */

#include <stdbool.h>
#include "../utils/log_floor.h"
#include "../utils/period_T.h"

/* auxiliary: compute integer square root (floor) */
static inline idx_t int_isqrt(idx_t x) {
    idx_t r = 0;
    while ((r+1)*(r+1) <= x) ++r;
    return r;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    /* find diagonal d and offset within it */
    idx_t d = 0;
    idx_t cum = 0;
    while (cum + (d+1) <= n) {
        cum += d+1;
        ++d;
    }
    idx_t offset = n - cum;           /* 0 ≤ offset ≤ d */

    /* coordinates (row, column) */
    idx_t r = offset;                 /* row index */
    idx_t k = d + 1 - offset;         /* column index (≥1) */

    return period_T(r, k);
}
